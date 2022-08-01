// Copyright Epic Games, Inc. All Rights Reserved.

#include "TabFileCommands.h"

#define LOCTEXT_NAMESPACE "FTabFileModule"

void FTabFileCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "TabFile", "Execute TabFile action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
