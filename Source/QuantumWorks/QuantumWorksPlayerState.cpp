// Fill out your copyright notice in the Description page of Project Settings.


#include "QuantumWorksPlayerState.h"

#include "Abilities/AttributeSets/QwAttributeSetBase.h"
#include "Abilities/QwAbilitySystemComponent.h"




AQuantumWorksPlayerState::AQuantumWorksPlayerState()
{
	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<UQwAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	// Mixed mode means we only are replicated the GEs to ourself, not the GEs to simulated proxies. If another ExWorldTestPlayerState (Hero) receives a GE,
	// we won't be told about it by the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Create the attribute set, this replicates by default
	// Adding it as a subobject of the owning actor of an AbilitySystemComponent
	// automatically registers the AttributeSet with the AbilitySystemComponent
	AttributeSetBase = CreateDefaultSubobject<UQwAttributeSetBase>(TEXT("AttributeSetBase"));

	// Set PlayerState's NetUpdateFrequency to the same as the Character.
	// Default is very low for PlayerStates and introduces perceived lag in the ability system.
	// 100 is probably way too high for a shipping game, you can adjust to fit your needs.
	NetUpdateFrequency = 100.0f;

}

UAbilitySystemComponent* AQuantumWorksPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UQwAttributeSetBase* AQuantumWorksPlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

void AQuantumWorksPlayerState::ShowAbilityConfirmCancelText(bool ShowText)
{

}

float AQuantumWorksPlayerState::GetHealth() const
{
	return AttributeSetBase->GetHealth();
}

float AQuantumWorksPlayerState::GetMaxHealth() const
{
	return AttributeSetBase->GetMaxHealth();
}

void AQuantumWorksPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}

}

void AQuantumWorksPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;


}

void AQuantumWorksPlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	float MaxHealth = Data.NewValue;


}

