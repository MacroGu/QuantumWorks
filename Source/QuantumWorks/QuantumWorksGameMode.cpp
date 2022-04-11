// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuantumWorksGameMode.h"
#include "QuantumWorksHUD.h"
#include "QuantumWorksCharacter.h"
#include "UObject/ConstructorHelpers.h"

AQuantumWorksGameMode::AQuantumWorksGameMode()
	: Super()
{
	// use our custom HUD class
	HUDClass = AQuantumWorksHUD::StaticClass();
}
