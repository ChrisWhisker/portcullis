#include "Portcullis.h"

#include "BSPGenerator.h"
#include "LevelEditor.h"
#include "PortcullisCommands.h"
#include "PortcullisStyle.h"
#include "ToolMenus.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

static const FName PortcullisTabName("Portcullis");

#define LOCTEXT_NAMESPACE "FPortcullisModule"

void FPortcullisModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FPortcullisStyle::Initialize();
	FPortcullisStyle::ReloadTextures();

	FPortcullisCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FPortcullisCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FPortcullisModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FPortcullisModule::RegisterMenus));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(PortcullisTabName,
	                                                  FOnSpawnTab::CreateRaw(
		                                                  this, &FPortcullisModule::OnSpawnPluginTab))
	                        .SetDisplayName(LOCTEXT("FPortcullisTabTitle", "Portcullis"))
	                        .SetMenuType(ETabSpawnerMenuType::Hidden);

	FPortcullisStyle::Initialize();
	FPortcullisStyle::ReloadTextures();

	FPortcullisCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FPortcullisCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FPortcullisModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FPortcullisModule::RegisterMenus));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(PortcullisTabName,
	                                                  FOnSpawnTab::CreateRaw(
		                                                  this, &FPortcullisModule::OnSpawnPluginTab))
	                        .SetDisplayName(LOCTEXT("FPortcullisTabTitle", "Portcullis"))
	                        .SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FPortcullisModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FPortcullisStyle::Shutdown();

	FPortcullisCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(PortcullisTabName);
}

TSharedRef<SDockTab> FPortcullisModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
            .HAlign(HAlign_Center)
            .VAlign(VAlign_Center)
			[
				SNew(SButton)
	            .Text(LOCTEXT("MyButtonLabel", "Generate"))
	            .OnClicked_Raw(this, &FPortcullisModule::OnButtonClick) // Bind the button click event
			]
		];
}

FReply FPortcullisModule::OnButtonClick()
{
	UE_LOG(LogTemp, Warning, TEXT("Generate button was clicked!"));

	if (UWorld* World = GEditor->GetEditorWorldContext().World())
	{
		// Instantiate BSPGenerator with the current world context
		FBSPGenerator BSPGen;
		BSPGen.Initialize(World);

		// Now you can call Generate or other functions as needed
		BSPGen.Generate();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get valid world context."));
	}
	return FReply::Handled();
}

void FPortcullisModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(PortcullisTabName);
}

void FPortcullisModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FPortcullisCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(
					FToolMenuEntry::InitToolBarButton(FPortcullisCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPortcullisModule, Portcullis)
