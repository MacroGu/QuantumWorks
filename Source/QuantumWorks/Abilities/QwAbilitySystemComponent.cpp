// Copyright Epic Games, Inc. All Rights Reserved.

#include "QwAbilitySystemComponent.h"
#include "QwGameplayAbility.h"
#include "AbilitySystemGlobals.h"


void UQwAbilitySystemComponent::ReceiveDamage(UQwAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}
