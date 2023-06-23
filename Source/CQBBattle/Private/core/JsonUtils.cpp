// Fill out your copyright notice in the Description page of Project Settings.


#include "core/JsonUtils.h"

TMap<FString, TSharedPtr<FJsonObject>> UJsonUtils::AllDatas;

FString UJsonUtils::GetGString(const FString& TableName, const FString& RowID, const FString& ColName)
{
	const TSharedPtr<FJsonObject> json = GetGData(TableName);
	if (!json.IsValid())
	{
		return TEXT("");
	}
	const TSharedPtr<FJsonObject>* data;
	json->TryGetObjectField(RowID, data);

	if (data->IsValid())
	{
		FString Value;
		data->Get()->TryGetStringField(ColName, Value);
		return Value;
	}
	return TEXT("");
}

TSharedPtr<FJsonObject> UJsonUtils::GetGData(const FString& TableName)
{
	TSharedPtr<FJsonObject> json = AllDatas.FindRef(TableName);
	return json;
}

int64 UJsonUtils::GetGInt(const FString& TableName, const FString& RowID, const FString& ColName)
{
	const TSharedPtr<FJsonObject> json = GetGData(TableName);
	if (!json.IsValid())
	{
		return 0;
	}
	const TSharedPtr<FJsonObject>* data;
	json->TryGetObjectField(RowID, data);

	if (data->IsValid())
	{
		int64 Value;
		data->Get()->TryGetNumberField(ColName, Value);
		return Value;
	}
	return 0;
}

bool UJsonUtils::ParseJson(const FString& RelPath)
{
	const TSharedPtr<FJsonObject> json = AllDatas.FindRef(RelPath);
	if (json.IsValid())
	{
		return true;
	}
	return false;
}
