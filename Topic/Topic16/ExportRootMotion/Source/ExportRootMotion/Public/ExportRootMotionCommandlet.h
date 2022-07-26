// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "ExportRootMotionCommandlet.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogCommandletExample, Log, All);

/**
 * Export UExport RootMotion Commandlet
 */
UCLASS()
class EXPORTROOTMOTION_API UExportRootMotionCommandlet : public UCommandlet
{
	GENERATED_BODY()

public:
	virtual int32 Main(const FString& Params) override;

};
