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
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;



protected:

	UFUNCTION()
	void OnTaskEnd();


	UFUNCTION()
	bool CheckDamageToOthers();


	// configure data
	//How long the player stays at the apex after the upward launch but before the downward launch.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float HangTime;

	// How far/how much force that launches the player upward.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float LaunchHeight;

	//A multiplying variable that multiplies the damage of the 	impact determined by how far OR how long the player 		travelled during the downward launch of the Ground Pound.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float DamageMultiplier;

	//The radial distance of the Ground Pound impact.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float ImpactRadius;

	// Determines how quickly the player reaches the apex of the movement.
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float UpwardLaunchSpeed;

	float LeftTimeHangTime;		// when the LeftTimeHangTime is 0, the ability should be over
	bool bAlreadyUpToApex;		// show if is at the apex of this ability


};
