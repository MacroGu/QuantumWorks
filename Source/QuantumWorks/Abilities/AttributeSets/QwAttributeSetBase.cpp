#include "QwAttributeSetBase.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UQwAttributeSetBase::UQwAttributeSetBase()
{
	// Cache tags

}

void UQwAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// This is called whenever attributes change, so for max health/mana we want to scale the current totals to match
	Super::PreAttributeChange(Attribute, NewValue);


	UE_LOG(LogTemp, Warning, TEXT("UQwAttributeSetBase::PreAttributeChange"));

}

void UQwAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
{
	Super::PostGameplayEffectExecute(Data);


	UE_LOG(LogTemp, Warning, TEXT("UQwAttributeSetBase::PostGameplayEffectExecute"));
}

void UQwAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UQwAttributeSetBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UQwAttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
}


void UQwAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UQwAttributeSetBase, Health, OldHealth);
}

void UQwAttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UQwAttributeSetBase, MaxHealth, OldMaxHealth);
}
