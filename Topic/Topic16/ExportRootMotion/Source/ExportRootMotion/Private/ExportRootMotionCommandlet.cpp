// Fill out your copyright notice in the Description page of Project Settings.


#include "ExportRootMotionCommandlet.h"
#include "Engine/ObjectLibrary.h"
#include "RMBlueprintFunctionLibrary.h"

int32 UExportRootMotionCommandlet::Main(const FString& Params)
{
    UE_LOG(LogCommandletExample, Log, TEXT("Example commandlet begin to execute ."));
    UE_LOG(LogCommandletExample, Log, TEXT("Input Param: %s"), *Params);

	TArray<FString> Tokens;
	TArray<FString> Switches;
	TMap<FString, FString> ParamsMap;
	ParseCommandLine(*Params, Tokens, Switches, ParamsMap);

	const FString& TmpAnimPath = TEXT("AnimPath");
	const FString& TmpFPS= TEXT("FPS");

	if (!ParamsMap.Contains(TmpAnimPath))
	{
		return 0;
	}

	FString AnimPath = ParamsMap[TmpAnimPath];
	
	UE_LOG(LogCommandletExample, Log, TEXT("Input Path: %s "), *AnimPath);

	UObjectLibrary* ObjectLibrary = UObjectLibrary::CreateLibrary(UAnimSequence::StaticClass(), false, GIsEditor);
	ObjectLibrary->LoadAssetDataFromPath(AnimPath);
	TArray<FAssetData> AssetDatas;
	ObjectLibrary->GetAssetDataList(AssetDatas);

	TArray<UObject*> Objs;
	UObject* Tmp;

	for (const FAssetData& AssetData : AssetDatas)
	{
		UE_LOG(LogCommandletExample, Log, TEXT("------- %s"), *AssetData.GetFullName());
		Tmp = AssetData.GetAsset();
		Objs.Add(Tmp);
	}

	URMBlueprintFunctionLibrary::ExportRootMotion(Objs);

    return 0;
}
