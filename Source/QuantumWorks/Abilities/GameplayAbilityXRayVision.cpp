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

	PC->SetNonFriendOutlineThickness(NonFriendlyOutlineThickness);
	PC->SetOthersOutlineThickness(OtherElementsThickness);


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
		if (!PrimitiveComponent)
		{
			continue;
		}

		float Distance = FVector::Dist(SelfLocation, OneActor->GetActorLocation());
		if (Distance > XRayDistance)
		{
			PrimitiveComponent->SetCustomDepthStencilValue(3);
		}
		else
		{
			AQwEnemyCharacter* EnemyCharacter = Cast<AQwEnemyCharacter>(OneActor);
			if (EnemyCharacter && EnemyCharacter->EntityTag == FGameplayTag::RequestGameplayTag(FName("Entities.NotFriendly")))
			{
				PrimitiveComponent->SetCustomDepthStencilValue(1);
			}
			else
			{
				PrimitiveComponent->SetCustomDepthStencilValue(2);
			}
		}
	}

	if (!Hero->GetIsRunningXRayVision())
	{
		for (auto OneActor : AllActors)
		{
			UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(OneActor->GetComponentByClass(UPrimitiveComponent::StaticClass()));
			if (!PrimitiveComponent)
			{
				continue;
			}
			PrimitiveComponent->SetCustomDepthStencilValue(3);
		}

		return true;
	}

	return false;
}