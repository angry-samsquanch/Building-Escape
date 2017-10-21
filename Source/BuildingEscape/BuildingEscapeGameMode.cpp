// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "BuildingEscapeGameMode.h"
#include "BuildingEscapeHUD.h"
#include "BuildingEscapeCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABuildingEscapeGameMode::ABuildingEscapeGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ABuildingEscapeHUD::StaticClass();
}
