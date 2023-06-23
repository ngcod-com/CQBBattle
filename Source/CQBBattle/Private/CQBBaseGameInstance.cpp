// Fill out your copyright notice in the Description page of Project Settings.


#include "CQBBaseGameInstance.h"

void UCQBBaseGameInstance::StartGameInstance()
{
	InitEngine();
	Super::StartGameInstance();
}

#if WITH_EDITOR
FGameInstancePIEResult UCQBBaseGameInstance::StartPlayInEditorGameInstance(ULocalPlayer* LocalPlayer, const FGameInstancePIEParameters& Params)
{
	FGameInstancePIEResult InitResult = FGameInstancePIEResult::Failure(FText());

	InitEngine();

	FGameInstancePIEResult StartResult = Super::StartPlayInEditorGameInstance(LocalPlayer, Params);

	return StartResult;
}
#endif

void UCQBBaseGameInstance::InitEngine()
{
	//初始化公共Lua和json
}