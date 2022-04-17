#include "GameplayAbilityXRayVision.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"

#include "QuantumWorksCharacter.h"
#include "Tasks/AbilityTask_WaitForPredicate.h"
#include "QwEnemyCharacter.h"
#include "QuantumWorksPlayerController.h"


UGameplayAbilityXRayVision::UGameplayAbilityXRayVision()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	FGameplayTag SpellAbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Skill.XRayVision"));
	AbilityTags.AddTag(SpellAbilityTag);
	ActivationOwnedTags.AddTag(SpellAbilityTag);

}

bool UGameplayAbilityXRayVision::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags , OUT FGameplayTagContainer* OptionalRelevantTags ) const
{

	return true;
}

void UGameplayAbilityXRayVision::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	auto DoingRayVision = [this](const UObject* Context, const float DeltaTime)
	{
		return DoRayVision();
	};

	UAbilityTask_WaitForPredicate* Task = UAbilityTask_WaitForPredicate::WaitForPredicate(this, Hero, DoingRayVision, -1.f);
	Task->OnFinish.AddDynamic(this, &ThisClass::OnTaskEnd);
	Task->ReadyForActivation();

}

void UGameplayAbilityXRayVision::OnTaskEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}


bool UGameplayAbilityXRayVision::DoRayVision()		// return true means end ability
{
	AQuantumWorksPlayerController* PC = Cast<AQuantumWorksPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!IsValid(PC))
	{
		return true;
	}

	PC->SetNonFriendOutlineThickness(NonFriendlyOutlineThickness);		// set thickness of nonfriend actor
	PC->SetOthersOutlineThickness(OtherElementsThickness);		// set thickness of others actor


	AQuantumWorksCharacter* Hero = Cast<AQuantumWorksCharacter>(GetAvatarActorFromActorInfo());
	if (!Hero)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return true;
	}
	const FVector SelfLocation = Hero->GetActorLocation();


	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);
	for (auto OneActor : AllActors)
	{
		UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(OneActor->GetComponentByClass(UPrimitiveComponent::StaticClass()));
		if (!PrimitiveComponent)		// only actor with primitive component can set outline
		{
			continue;
		}

		float Distance = FVector::Dist(SelfLocation, OneActor->GetActorLocation());
		if (Distance > XRayDistance)		// if the actor range is bigger than configure, the actor can not be outline
		{
			PrimitiveComponent->SetCustomDepthStencilValue((uint8)EXRay::CannotVision);
		}
		else
		{
			AQwEnemyCharacter* EnemyCharacter = Cast<AQwEnemyCharacter>(OneActor);
			if (EnemyCharacter)
			{
				if (EnemyCharacter->EntityTag == FGameplayTag::RequestGameplayTag(FName("Entities.NonFriendly")))
				{
					PrimitiveComponent->SetCustomDepthStencilValue((uint8)EXRay::NonFriendly);		// set outline for NonFriendly
				}
				else if (EnemyCharacter->EntityTag == FGameplayTag::RequestGameplayTag(FName("Entities.Friendly")))
				{
					PrimitiveComponent->SetCustomDepthStencilValue((uint8)EXRay::Friendly);		// set outline for NonFriendly
				}
			}
			else
			{
				PrimitiveComponent->SetCustomDepthStencilValue((uint8)EXRay::Others);		// set outline for others actor
			}
		}
	}

	if (!Hero->GetIsRunningXRayVision())		// if the ability is stop, let all outline disappear
	{
		for (auto OneActor : AllActors)
		{
			UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(OneActor->GetComponentByClass(UPrimitiveComponent::StaticClass()));
			if (!PrimitiveComponent)
			{
				continue;
			}
			PrimitiveComponent->SetCustomDepthStencilValue((uint8)EXRay::CannotVision);
		}

		return true;
	}

	return false;
}