// Fill out your copyright notice in the Description page of Project Settings.


#include "CQBBasePlayerController.h"

#include "GameFramework/Character.h"

void ACQBBasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &ACQBBasePlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACQBBasePlayerController::MoveRight);
}

void ACQBBasePlayerController::MoveForward(float AxisValue)
{
	MoveInput.X = AxisValue;
}

void ACQBBasePlayerController::MoveRight(float AxisValue)
{
	MoveInput.Y = AxisValue;
}

void ACQBBasePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!MoveInput.IsNearlyZero())
	{
		auto rotator = GetControlRotation();
		rotator.Roll = 0;
		rotator.Pitch = 0;
		ACharacter* myCharacter = this->GetCharacter();
		FVector RightVector = FRotationMatrix(rotator).GetScaledAxis(EAxis::Y);
		FVector ForwardVector = FRotationMatrix(rotator).GetScaledAxis(EAxis::X);
		myCharacter->AddMovementInput(ForwardVector, MoveInput.X, false);
		myCharacter->AddMovementInput(RightVector, MoveInput.Y, false);
	}
}

