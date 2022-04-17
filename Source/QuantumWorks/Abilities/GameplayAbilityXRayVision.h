#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "QwGameplayAbility.h"
#include "GameplayAbilityXRayVision.generated.h"


/**
 *	Ability that Spell.
 */

UCLASS()
class QUANTUMWORKS_API UGameplayAbilityXRayVision : public UQwGameplayAbility
{
	GENERATED_BODY()

public:

	UGameplayAbilityXRayVision();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;



protected:


	// configure data
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float XRayDistance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float NonFriendlyOutlineThickness;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float OtherElementsThickness;


};
