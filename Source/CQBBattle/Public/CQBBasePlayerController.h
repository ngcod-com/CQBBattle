// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CQBBasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CQBBATTLE_API ACQBBasePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void SetupInputComponent() override;
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	virtual void Tick(float DeltaSeconds) override;
private:
	FVector2D MoveInput;
};
