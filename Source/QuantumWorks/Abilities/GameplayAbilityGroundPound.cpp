#include "GameplayAbilityGroundPound.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameFramework/CharacterMovementComponent.h"


#include "QuantumWorksCharacter.h"
#include "Tasks/AbilityTask_WaitForPredicate.h"



UGameplayAbilityGroundPound::UGameplayAbilityGroundPound()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	FGameplayTag SpellAbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Skill.GroundPound"));
	AbilityTags.AddTag(SpellAbilityTag);
	ActivationOwnedTags.AddTag(SpellAbilityTag);

}

bool UGameplayAbilityGroundPound::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags , OUT FGameplayTagContainer* OptionalRelevantTags ) const
{

	return true;
}

void UGameplayAbilityGroundPound::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, false);
		return;
	}

	AQuantumWorksCharacter* Hero = Cast<AQuantumWorksCharacter>(GetAvatarActorFromActorInfo());
	if (!Hero)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	FVector LaunchVelocity(0.f, 0.f, UpwardLaunchSpeed);
	Hero->LaunchCharacter(LaunchVelocity, true, true);

	LeftTimeHangTime = HangTime;
	auto LaunchFlowFinished = [this](const UObject* Context, const float DeltaTime)
	{
		AQuantumWorksCharacter* Character = Cast<AQuantumWorksCharacter>(GetAvatarActorFromActorInfo());
		if (!Character)
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
			return false;
		}
		FVector UpdateLocation = Character->GetActorLocation();
		if (UpdateLocation.Z < LaunchHeight)
		{
			return false;
		}
		else
		{
			LeftTimeHangTime = LeftTimeHangTime - DeltaTime;
			if (LeftTimeHangTime > 0)
			{
				Character->GetCharacterMovement()->DisableMovement();
				return false;
			}
			else
			{
				return true;
			}
		}
	};

	UAbilityTask_WaitForPredicate* Task = UAbilityTask_WaitForPredicate::WaitForPredicate(this, Hero, LaunchFlowFinished, -1.f);
	Task->OnFinish.AddDynamic(this, &ThisClass::OnTaskEnd);
	Task->ReadyForActivation();


	Hero->GetCharacterMovement()->GravityScale = 0.f;
}

void UGameplayAbilityGroundPound::OnTaskEnd()
{
	AQuantumWorksCharacter* Hero = Cast<AQuantumWorksCharacter>(GetAvatarActorFromActorInfo());
	if (!Hero)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	Hero->GetCharacterMovement()->GravityScale = 1.f;
	Hero->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
