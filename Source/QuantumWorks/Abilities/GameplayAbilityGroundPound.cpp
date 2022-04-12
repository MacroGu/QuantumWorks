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

	FVector NewLocation = Hero->GetActorLocation();
	NewLocation.Z += LaunchHeight;
	Hero->SetActorLocation(NewLocation);
	LeftTimeHangTime = HangTime;
	auto LaunchFlowFinished = [this](const UObject* Context, const float DeltaTime)
	{
		LeftTimeHangTime = LeftTimeHangTime - DeltaTime;
		if (LeftTimeHangTime > 0)
		{
			return false;
		}
		UE_LOG(LogTemp, Warning, TEXT("UGameplayAbilityGroundPound::ActivateAbility %f"), DeltaTime);

		return true;
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

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
