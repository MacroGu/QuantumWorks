// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "QuantumWorks.h"
#include "AbilitySystemComponent.h"
#include "QwGameplayAbility.h"
#include "QwAbilitySystemComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, UQwAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage);


/**
 * Subclass of ability system component with game-specific data
 * Most games will need to make a game-specific subclass to provide utility functions
 */

UCLASS()
class QUANTUMWORKS_API UQwAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	bool CharacterAbilitiesGiven = false;
	bool StartupEffectsApplied = false;

	FReceivedDamageDelegate ReceivedDamage;

	// Called from QwDamageExecCalculation. Broadcasts on ReceivedDamage whenever this ASC receives damage.
	virtual void ReceiveDamage(UQwAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);


};