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

	// set the ability tag 
	FGameplayTag SpellAbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Skill.GroundPound"));
	AbilityTags.AddTag(SpellAbilityTag);
	ActivationOwnedTags.AddTag(SpellAbilityTag);

}

bool UGameplayAbilityGroundPound::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags , OUT FGameplayTagContainer* OptionalRelevantTags ) const
{

	// currently it can be activated any time
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

	AQuantumWorksCharacter* Hero = Cast<AQuantumWorksCharacter>(GetAvatarActorFromActorInfo());		// get controlled character
	if (!Hero)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}

	FVector LaunchVelocity(0.f, 0.f, UpwardLaunchSpeed);
	Hero->LaunchCharacter(LaunchVelocity, true, true);		// launch the character

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
			return false;		// the character still in the process up to the apex during spell this ability
		}
		else
		{
			bAlreadyUpToApex = true;
			LeftTimeHangTime = LeftTimeHangTime - DeltaTime;  // the character still in the process down to the floor
			if (LeftTimeHangTime > 0)	
			{
				Character->GetCharacterMovement()->DisableMovement();		// the character can not move during the ability
				return false;
			}
			else
			{
				return CheckDamageToOthers();		// the character is at the floor, and check any damage to the others around
			}
		}
	};

	UAbilityTask_WaitForPredicate* Task = UAbilityTask_WaitForPredicate::WaitForPredicate(this, Hero, LaunchFlowFinished, -1.f);
	Task->OnFinish.AddDynamic(this, &ThisClass::OnTaskEnd);
	Task->ReadyForActivation();


	Hero->GetCharacterMovement()->GravityScale = 0.f;			// can not down, when launch
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

	if (Hero->GetCharacterMovement()->MovementMode != EMovementMode::MOVE_Falling)		// change the character status when active the ability
	{
		Hero->GetCharacterMovement()->GravityScale = 1.f;
		Hero->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		return false;
	}

	if (FMath::Abs(Hero->GetActorLocation().Z) > 2.f)		// on floor  check character
	{
		return false;
	}

	Hero->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);		// when the character is on the floor, change the status of character
	const FVector SelfLocation = Hero->GetActorLocation();
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);
	for (auto OneActor : AllActors)
	{
		AQwEnemyCharacter* EnemyCharacter = Cast<AQwEnemyCharacter>(OneActor);

		if (!EnemyCharacter)
		{
			continue;
		}

		if (EnemyCharacter->EntityTag != FGameplayTag::RequestGameplayTag(FName("Entities.NonFriendly")))		// can not damage to not NonFriendly actors
		{
			continue;
		}

		float Distance = FVector::Dist(SelfLocation, EnemyCharacter->GetActorLocation());
		if (Distance > ImpactRadius)		// check the distance between the damaged character and self character, if can aplly the damage
		{
			continue;
		}

		EnemyCharacter->ReceiveDamage(DamageMultiplier * LaunchHeight / Distance);  // let the damage character receive damage, the distance is big the damage will be small, and the LaunchHeight is bigger , the damage will be bigger
	}

	return true;
}
