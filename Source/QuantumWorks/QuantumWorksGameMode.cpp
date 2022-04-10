// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuantumWorksGameMode.h"
#include "QuantumWorksHUD.h"
#include "QuantumWorksCharacter.h"
#include "UObject/ConstructorHelpers.h"

AQuantumWorksGameMode::AQuantumWorksGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AQuantumWorksHUD::StaticClass();
}
