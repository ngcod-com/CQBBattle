// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CQBBaseGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CQBBATTLE_API UCQBBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	void InitEngine();
	virtual void StartGameInstance() override;
#if WITH_EDITOR
	/* Called to actually start the game when doing Play/Simulate In Editor */
	virtual FGameInstancePIEResult StartPlayInEditorGameInstance(ULocalPlayer* LocalPlayer, const FGameInstancePIEParameters& Params) override;
#endif
};
