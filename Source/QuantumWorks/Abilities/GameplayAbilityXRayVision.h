#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "QwGameplayAbility.h"
#include "GameplayAbilityXRayVision.generated.h"


/**
 *	Ability that x-ray vision.
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

	UFUNCTION()
	void OnTaskEnd();		// this ability is end

	bool DoRayVision();		// do the ray vision, that means let the objects outline

protected:


	// configure data
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float XRayDistance;			// the range that can apply the x ray vision ability

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float NonFriendlyOutlineThickness;		// Determines the thickness of the outlines of ¡®Non-Friendly¡¯ actors

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float OtherElementsThickness;		// Determines the thickness of the outlines of all other elements

};
