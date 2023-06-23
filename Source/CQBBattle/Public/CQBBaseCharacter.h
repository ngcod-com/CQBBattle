// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CQBBaseCharacter.generated.h"

UCLASS()
class CQBBATTLE_API ACQBBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACQBBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Load();
public:
	UFUNCTION(BlueprintCallable)
	void SetResTypeID(const int64 ResID);

	UFUNCTION(BlueprintCallable)
	FString GetAnimRes(const FString& AnimName);
private:
	int64 ResTypeID;
};
