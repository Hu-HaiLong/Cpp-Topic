// Copyright Epic Games, Inc. All Rights Reserved.

#include "TabFile.h"
#include "TabFileStyle.h"
#include "TabFileCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "TestTabFile.h"

static const FName TabFileTabName("TabFile");

#define LOCTEXT_NAMESPACE "FTabFileModule"

void FTabFileModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FTabFileStyle::Initialize();
	FTabFileStyle::ReloadTextures();

	FTabFileCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FTabFileCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FTabFileModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FTabFileModule::RegisterMenus));
}

void FTabFileModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FTabFileStyle::Shutdown();

	FTabFileCommands::Unregister();
}

void FTabFileModule::PluginButtonClicked()
{
	// Example Test
	FSceneTabFile Test = FSceneTabFile();

	for (auto& Data : Test.GetDatas())
	{
		UE_LOG(LogTemp, Log, TEXT("Test:  %d, %s"), Data.Value.SceneId, *Data.Value.Descriptors);
	}
}

void FTabFileModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FTabFileCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FTabFileCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTabFileModule, TabFile)