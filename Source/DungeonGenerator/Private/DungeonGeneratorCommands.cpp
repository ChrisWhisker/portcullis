// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeonGeneratorCommands.h"

#define LOCTEXT_NAMESPACE "FDungeonGeneratorModule"

void FDungeonGeneratorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "DungeonGenerator", "Bring up DungeonGenerator window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
