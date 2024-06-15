// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "PortcullisStyle.h"

class FPortcullisCommands : public TCommands<FPortcullisCommands>
{
public:

	FPortcullisCommands()
		: TCommands<FPortcullisCommands>(TEXT("Portcullis"), NSLOCTEXT("Contexts", "Portcullis", "Portcullis Plugin"), NAME_None, FPortcullisStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};