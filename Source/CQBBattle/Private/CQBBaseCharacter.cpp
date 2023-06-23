// Fill out your copyright notice in the Description page of Project Settings.


#include "CQBBaseCharacter.h"

#include "core/JsonUtils.h"
#include "CQBBattle/GameDefine.h"

// Sets default values
ACQBBaseCharacter::ACQBBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACQBBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	Load();
}

// Called every frame
void ACQBBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACQBBaseCharacter::Load()
{
	//根据ResTypeID获取模型，动作等资源
	//这里通过json配置表查询
	// const FString MeshRes = UJsonUtils::GetGString(TABLE_ROLE_ANIM, FString::FromInt(ResTypeID), TEXT("rmesh"));
	// const FString AnimRes = UJsonUtils::GetGString(TABLE_ROLE_ANIM,FString::FromInt( ResTypeID), TEXT("ranim"));

	const FString MeshRes = TEXT("/Game/test/Role/Alex/Alex.Alex");
	const FString AnimRes = TEXT("/Game/Core/BP_Animation.BP_Animation_C");
	
	USkeletalMeshComponent* MeshComponent = GetMesh();
	//加载Mesh资源
	UObject* MeshAsset = LoadObject<UObject>(nullptr, *MeshRes);
	USkeletalMesh* SkeletonMesh =  Cast<USkeletalMesh>(MeshAsset);
	MeshComponent->SetSkeletalMesh(SkeletonMesh, true);

	//加载通用动画蓝图并设置Skeleton(骨架)
	UClass* AnimClass = LoadClass<UObject>(nullptr, *AnimRes);
	UAnimBlueprintGeneratedClass* TempAnimClass = Cast<UAnimBlueprintGeneratedClass>(AnimClass);
	if (TempAnimClass)
	{
		TempAnimClass->TargetSkeleton = SkeletonMesh->GetSkeleton();
	}
	MeshComponent->SetAnimClass(AnimClass);
}

void ACQBBaseCharacter::SetResTypeID(const int64 ResID)
{
	this->ResTypeID = ResID;
}

FString ACQBBaseCharacter::GetAnimRes(const FString& AnimName)
{
	const FString AnimRes = UJsonUtils::GetGString(TABLE_ROLE_ANIM, FString::FromInt(ResTypeID), AnimName);
	return AnimRes;
}

