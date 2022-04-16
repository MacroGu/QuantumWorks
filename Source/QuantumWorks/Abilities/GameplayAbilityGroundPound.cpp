#include "GameplayAbilityGroundPound.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"

#include "QuantumWorksCharacter.h"
#include "Tasks/AbilityTask_WaitForPredicate.h"
#include "QwEnemyCharacter.h"


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

	bAlreadyUpToApex = false;
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

		if (UpdateLocation.Z < LaunchHeight && !bAlreadyUpToApex)
		{
			return false;
		}
		else
		{
			bAlreadyUpToApex = true;
			LeftTimeHangTime = LeftTimeHangTime - DeltaTime;
			if (LeftTimeHangTime > 0)
			{
				Character->GetCharacterMovement()->DisableMovement();
				return false;
			}
			else
			{
				return CheckDamageToOthers();
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

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

bool UGameplayAbilityGroundPound::CheckDamageToOthers()
{
	AQuantumWorksCharacter* Hero = Cast<AQuantumWorksCharacter>(GetAvatarActorFromActorInfo());
	if (!Hero)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return false;
	}

	if (Hero->GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Falling)
	{
		Hero->GetCharacterMovement()->GravityScale = 1.f;
		Hero->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		return false;
	}

	if (FMath::Abs(Hero->GetActorLocation().Z) > 2.f)		// on floor
	{
		return false;
	}

	Hero->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	const FVector SelfLocation = Hero->GetActorLocation();
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);
	for (auto OneActor : AllActors)
	{
		AQwEnemyCharacter* EnemyCharacter = Cast<AQwEnemyCharacter>(OneActor);
		if (!EnemyCharacter) continue;
		float Distance = FVector::Dist(SelfLocation, EnemyCharacter->GetActorLocation());
		if (Distance > ImpactRadius) continue;
		EnemyCharacter->ReceiveDamage(DamageMultiplier * 100);
	}

	return true;
}
