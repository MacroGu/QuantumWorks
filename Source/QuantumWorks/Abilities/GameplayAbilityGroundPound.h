#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "QwGameplayAbility.h"
#include "GameplayAbilityGroundPound.generated.h"


/**
 *	Ability that Spell.
 */

UCLASS()
class QUANTUMWORKS_API UGameplayAbilityGroundPound : public UQwGameplayAbility
{
	GENERATED_BODY()

public:

	UGameplayAbilityGroundPound();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UAnimMontage* SpellMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Range;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Damage;


};
