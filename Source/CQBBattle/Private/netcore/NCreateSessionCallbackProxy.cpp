// Fill out your copyright notice in the Description page of Project Settings.


#include "netcore/NCreateSessionCallbackProxy.h"

#include "Engine/Engine.h"
#include "OnlineSubsystem.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "OnlineSubsystemUtils.h"

FMyOnlineSubsystemBPCallHelper::FMyOnlineSubsystemBPCallHelper(const TCHAR* CallFunctionContext, UObject* WorldContextObject, FName SystemName)
	: OnlineSub(Online::GetSubsystem(GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull), SystemName))
	, FunctionContext(CallFunctionContext)
{
	if (OnlineSub == nullptr)
	{
		FFrame::KismetExecutionMessage(*FString::Printf(TEXT("%s - Invalid or uninitialized OnlineSubsystem"), FunctionContext), ELogVerbosity::Warning);
	}
}

void FMyOnlineSubsystemBPCallHelper::QueryIDFromPlayerController(APlayerController* PlayerController)
{
	UserID.Reset();

	if (APlayerState* PlayerState = (PlayerController != NULL) ? PlayerController->PlayerState : NULL)
	{
		UserID = PlayerState->GetUniqueId().GetUniqueNetId();
		if (!UserID.IsValid())
		{
			FFrame::KismetExecutionMessage(*FString::Printf(TEXT("%s - Cannot map local player to unique net ID"), FunctionContext), ELogVerbosity::Warning);
		}
	}
	else
	{
		FFrame::KismetExecutionMessage(*FString::Printf(TEXT("%s - Invalid player state"), FunctionContext), ELogVerbosity::Warning);
	}
}

//////////////////////////////////////////////////////////////////////////
// UCreateSessionCallbackProxy

UNCreateSessionCallbackProxy::UNCreateSessionCallbackProxy(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
	, CreateCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateCompleted))
	, StartCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartCompleted))
	, NumPublicConnections(1)
{
}

UNCreateSessionCallbackProxy* UNCreateSessionCallbackProxy::CreateSession(UObject* WorldContextObject, class APlayerController* PlayerController, int32 PublicConnections
	, bool bUseLAN, FString Ext)
{
	UNCreateSessionCallbackProxy* Proxy = NewObject<UNCreateSessionCallbackProxy>();
	Proxy->PlayerControllerWeakPtr = PlayerController;
	Proxy->NumPublicConnections = PublicConnections;
	Proxy->bUseLAN = bUseLAN;
	Proxy->Ext = Ext;
	Proxy->WorldContextObject = WorldContextObject;
	return Proxy;
}

void UNCreateSessionCallbackProxy::Activate()
{
	FMyOnlineSubsystemBPCallHelper Helper(TEXT("CreateSession"), WorldContextObject);
	Helper.QueryIDFromPlayerController(PlayerControllerWeakPtr.Get());

	if (Helper.IsValid())
	{
		auto Sessions = Helper.OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			CreateCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(CreateCompleteDelegate);

			FOnlineSessionSettings Settings;
			Settings.NumPublicConnections = NumPublicConnections;
			Settings.bShouldAdvertise = true;
			Settings.bAllowJoinInProgress = true;
			Settings.bIsLANMatch = bUseLAN;
			Settings.bUsesPresence = true;
			Settings.bAllowJoinViaPresence = true;
			Settings.Settings.Add("ngcod", FOnlineSessionSetting(Ext, EOnlineDataAdvertisementType::ViaOnlineService));
			Sessions->CreateSession(*Helper.UserID, NAME_GameSession, Settings);

			// OnCreateCompleted will get called, nothing more to do now
			return;
		}
		else
		{
			FFrame::KismetExecutionMessage(TEXT("Sessions not supported by Online Subsystem"), ELogVerbosity::Warning);
		}
	}

	// Fail immediately
	OnFailure.Broadcast();
}

void UNCreateSessionCallbackProxy::OnCreateCompleted(FName SessionName, bool bWasSuccessful)
{
	FMyOnlineSubsystemBPCallHelper Helper(TEXT("CreateSessionCallback"), WorldContextObject);
	Helper.QueryIDFromPlayerController(PlayerControllerWeakPtr.Get());

	if (Helper.IsValid())
	{
		auto Sessions = Helper.OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(CreateCompleteDelegateHandle);

			if (bWasSuccessful)
			{
				StartCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(StartCompleteDelegate);
				Sessions->StartSession(NAME_GameSession);

				// OnStartCompleted will get called, nothing more to do now
				return;
			}
		}
	}

	if (!bWasSuccessful)
	{
		OnFailure.Broadcast();
	}
}

void UNCreateSessionCallbackProxy::OnStartCompleted(FName SessionName, bool bWasSuccessful)
{
	FMyOnlineSubsystemBPCallHelper Helper(TEXT("StartSessionCallback"), WorldContextObject);
	Helper.QueryIDFromPlayerController(PlayerControllerWeakPtr.Get());

	if (Helper.IsValid())
	{
		auto Sessions = Helper.OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(StartCompleteDelegateHandle);
		}
	}

	if (bWasSuccessful)
	{
		OnSuccess.Broadcast();
	}
	else
	{
		OnFailure.Broadcast();
	}
}
