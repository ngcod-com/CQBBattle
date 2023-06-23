// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CQBBaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CQBBATTLE_API UCQBBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category="CQB|Anim")
	UObject* GetAnimAsset(const FString& Key, const FString& Path);

	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe), Category="CQB|Anim")
	UObject* GetAnimAsset4StateMachine(const FString& Key);

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeBeginPlay() override;
public:
	UPROPERTY(BlueprintReadWrite)
	float Speed;
	UPROPERTY(BlueprintReadWrite)
	float Direction;
private:
	TMap<FString, UObject*> Anims;
};
