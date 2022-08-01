// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "TabFileStyle.h"

class FTabFileCommands : public TCommands<FTabFileCommands>
{
public:

    FTabFileCommands()
        : TCommands<FTabFileCommands>(TEXT("TabFile"), NSLOCTEXT("Contexts", "TabFile", "TabFile Plugin"), NAME_None, FTabFileStyle::GetStyleSetName())
    {
    }

    // TCommands<> interface
    virtual void RegisterCommands() override;

public:
    TSharedPtr< FUICommandInfo > PluginAction;
};
