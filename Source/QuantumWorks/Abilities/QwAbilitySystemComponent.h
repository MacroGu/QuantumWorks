// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "QuantumWorks.h"
#include "AbilitySystemComponent.h"
#include "QwGameplayAbility.h"
#include "QwAbilitySystemComponent.generated.h"


/**
 * Subclass of ability system component with game-specific data
 * Most games will need to make a game-specific subclass to provide utility functions
 */

UCLASS()
class QUANTUMWORKS_API UQwAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	bool CharacterAbilitiesGiven = false;		// if active ability when give the ability

};