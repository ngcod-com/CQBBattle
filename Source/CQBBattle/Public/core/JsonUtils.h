// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JsonUtils.generated.h"

/**
 * 
 */
UCLASS()
class CQBBATTLE_API UJsonUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static FString GetGString(const FString& TableName, const FString& RowID, const FString& ColName);

	static TSharedPtr<FJsonObject> GetGData(const FString& TableName);

	
	UFUNCTION(BlueprintCallable)
	static int64 GetGInt(const FString& TableName, const FString& RowID, const FString& ColName);
	
	UFUNCTION(BlueprintCallable)
	static bool ParseJson(const FString& RelPath);
private:
	static TMap<FString, TSharedPtr<FJsonObject>> AllDatas;
};
