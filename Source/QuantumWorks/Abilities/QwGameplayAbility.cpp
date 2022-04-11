// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/QwGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"



UQwGameplayAbility::UQwGameplayAbility()
{
	// Default to Instance Per Actor
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}

void UQwGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActivateAbilityOnGranted)
	{
		bool ActivatedAbility = ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}