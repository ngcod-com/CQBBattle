// Fill out your copyright notice in the Description page of Project Settings.


#include "CQBBaseAnimInstance.h"

#include "CQBBaseCharacter.h"
#include "CQBBattle/GameDefine.h"
#include "Kismet/KismetMathLibrary.h"

UObject* UCQBBaseAnimInstance::GetAnimAsset(const FString& Key, const FString& Path)
{
	UObject* AnimAsset = Anims.FindRef(Key);
	if (nullptr != AnimAsset)
	{
		return AnimAsset;
	}
	AnimAsset = LoadObject<UObject>(NULL, *Path);
	if (nullptr != AnimAsset)
	{
		Anims.Emplace(Key, AnimAsset);
	}
	return AnimAsset;
}

UObject* UCQBBaseAnimInstance::GetAnimAsset4StateMachine(const FString& Key)
{
	UObject* AnimAsset = Anims.FindRef(Key);
	return AnimAsset;
}

void UCQBBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	APawn* pawn = TryGetPawnOwner();
	if (nullptr == pawn)
	{
		return;
	}
	ACQBBaseCharacter* CQBCharacter = Cast<ACQBBaseCharacter>(pawn);
	// const FString AnimPath = CQBCharacter->GetAnimRes(ANIM_NAME_WALK);
	const FString AnimPath = TEXT("/Game/test/Role/Alex/BS_Alex.BS_Alex");
	GetAnimAsset(ANIM_NAME_WALK, AnimPath);
	FVector Velocity = pawn->GetVelocity();
	Speed = Velocity.Size();
	if (Speed > 0)
	{
		Velocity.Normalize();
		FRotator ControlRot = pawn->GetControlRotation();
		ControlRot.Roll = 0;
		ControlRot.Pitch = 0;

		const FVector RightVec = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
		const FVector  ForwardVec = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::X);

		const float dpRight = Velocity.operator|(RightVec);
		const float dpForward = Velocity.operator|(ForwardVec);
		const float VelAngle = UKismetMathLibrary::Acos(dpForward);

		const float Dir = dpRight>0?VelAngle:-VelAngle;
		Direction = Dir;
	}
}

void UCQBBaseAnimInstance::NativeBeginPlay()
{
	const APawn* pawn = TryGetPawnOwner();
	if (nullptr != pawn)
	{
		const FString AnimPath = TEXT("/Game/test/Role/Alex/BS_Alex.BS_Alex");
		GetAnimAsset(ANIM_NAME_WALK, AnimPath);
	}
}
