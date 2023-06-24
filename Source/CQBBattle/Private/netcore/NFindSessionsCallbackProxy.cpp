// Fill out your copyright notice in the Description page of Project Settings.


#include "netcore/NFindSessionsCallbackProxy.h"
#include "netcore/NCreateSessionCallbackProxy.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"

UNFindSessionsCallbackProxy::UNFindSessionsCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, Delegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnCompleted))
	, bUseLAN(false)
{
}

UNFindSessionsCallbackProxy* UNFindSessionsCallbackProxy::FindSessions(UObject* WorldContextObject, class APlayerController* PlayerController, int MaxResults, bool bUseLAN)
{
	UNFindSessionsCallbackProxy* Proxy = NewObject<UNFindSessionsCallbackProxy>();
	Proxy->PlayerControllerWeakPtr = PlayerController;
	Proxy->bUseLAN = bUseLAN;
	Proxy->MaxResults = MaxResults;
	Proxy->WorldContextObject = WorldContextObject;
	return Proxy;
}

void UNFindSessionsCallbackProxy::Activate()
{
	FMyOnlineSubsystemBPCallHelper Helper(TEXT("FindSessions"), WorldContextObject);
	Helper.QueryIDFromPlayerController(PlayerControllerWeakPtr.Get());

	if (Helper.IsValid())
	{
		auto Sessions = Helper.OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			DelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(Delegate);
			
			SearchObject = MakeShareable(new FOnlineSessionSearch);
			SearchObject->MaxSearchResults = MaxResults;
			SearchObject->bIsLanQuery = bUseLAN;
			SearchObject->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

			Sessions->FindSessions(*Helper.UserID, SearchObject.ToSharedRef());

			// OnQueryCompleted will get called, nothing more to do now
			return;
		}
		else
		{
			FFrame::KismetExecutionMessage(TEXT("Sessions not supported by Online Subsystem"), ELogVerbosity::Warning);
		}
	}

	// Fail immediately
	TArray<FBlueprintSessionResult> Results;
	OnFailure.Broadcast(Results);
}

void UNFindSessionsCallbackProxy::OnCompleted(bool bSuccess)
{
	FMyOnlineSubsystemBPCallHelper Helper(TEXT("FindSessionsCallback"), WorldContextObject);
	Helper.QueryIDFromPlayerController(PlayerControllerWeakPtr.Get());

	if (Helper.IsValid())
	{
		auto Sessions = Helper.OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(DelegateHandle);
		}
	}

	TArray<FBlueprintSessionResult> Results;

	if (bSuccess && SearchObject.IsValid())
	{
		for (auto& Result : SearchObject->SearchResults)
		{
			FBlueprintSessionResult BPResult;
			BPResult.OnlineResult = Result;
			Results.Add(BPResult);
		}

		OnSuccess.Broadcast(Results);
	}
	else
	{
		OnFailure.Broadcast(Results);
	}
}

int32 UNFindSessionsCallbackProxy::GetPingInMs(const FBlueprintSessionResult& Result)
{
	return Result.OnlineResult.PingInMs;
}

FString UNFindSessionsCallbackProxy::GetServerName(const FBlueprintSessionResult& Result)
{
	return Result.OnlineResult.Session.OwningUserName;
}

int32 UNFindSessionsCallbackProxy::GetCurrentPlayers(const FBlueprintSessionResult& Result)
{
	return Result.OnlineResult.Session.SessionSettings.NumPublicConnections - Result.OnlineResult.Session.NumOpenPublicConnections;
}

int32 UNFindSessionsCallbackProxy::GetMaxPlayers(const FBlueprintSessionResult& Result)
{
	return Result.OnlineResult.Session.SessionSettings.NumPublicConnections;
}

FString UNFindSessionsCallbackProxy::GetRoomName(const FBlueprintSessionResult& Result)
{
	return Result.OnlineResult.Session.SessionSettings.Settings.FindRef(TEXT("ngcod")).Data.ToString();
}


