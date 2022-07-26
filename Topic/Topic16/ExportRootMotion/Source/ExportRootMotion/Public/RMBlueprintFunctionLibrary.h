// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RMBlueprintFunctionLibrary.generated.h"

USTRUCT()
struct FAnimTrackStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		float Time;

	UPROPERTY()
		float X;

	UPROPERTY()
		float Y;

	UPROPERTY()
		float Z;

	UPROPERTY()
		float Yaw;

};


USTRUCT()
struct FAnimStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		FString Path;

	UPROPERTY()
		FString AnimName;

	UPROPERTY()
		TArray<FAnimTrackStruct> Data;

};

USTRUCT()
struct FExportStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		TArray<FAnimStruct> Data;
};

/**
 * 
 */
UCLASS()
class EXPORTROOTMOTION_API URMBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "RootMotion")
	static void ExportRootMotion(TArray<UObject*> Objs);
};
