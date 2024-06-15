// Copyright Epic Games, Inc. All Rights Reserved.

#include "PortcullisCommands.h"

#define LOCTEXT_NAMESPACE "FPortcullisModule"

void FPortcullisCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Portcullis", "Bring up Portcullis window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
