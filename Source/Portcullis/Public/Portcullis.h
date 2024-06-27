#pragma once

#include "CoreMinimal.h"
#include "GeneratorSettings.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;
#include "ToolMenus.h"

class FPortcullisModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();

private:
	/** Registers the plugin menus. */
	void RegisterMenus();

	/** Callback for when the plugin tab is spawned. */
	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

	/** Callback for button click event. */
	FReply OnButtonClick();

	TSharedPtr<class FUICommandList> PluginCommands;

	/** Callback for checkbox state change event for "Show partitions". */
	void OnShowPartitionsChanged(const ECheckBoxState NewState);

	/** Callback for checkbox state change event for "Show rooms". */
	void OnShowRoomsChanged(const ECheckBoxState NewState);

	FGeneratorSettings Settings;
};
