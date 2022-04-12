#include "AbilityTask_WaitForPredicate.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UAbilityTask_WaitForPredicate::UAbilityTask_WaitForPredicate(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TimeOut = 0.f;

	bTickingTask = true;
}

void UAbilityTask_WaitForPredicate::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (Predicate(Object, DeltaTime))
	{
		OnFinish.Broadcast();
		EndTask();
	}


}

UAbilityTask_WaitForPredicate* UAbilityTask_WaitForPredicate::WaitForPredicate(UGameplayAbility* OwningAbility, UObject* Object, TObjectPredicate Predicate, float TimeOut /*= 0.0f*/)
{
	UAbilityTask_WaitForPredicate* Task = NewAbilityTask<UAbilityTask_WaitForPredicate>(OwningAbility);
	Task->TimeOut = TimeOut;
	Task->Predicate = Predicate;
	Task->Object = Object;

	return Task;
}

void UAbilityTask_WaitForPredicate::Activate()
{
	UWorld* World = GetWorld();
	TimeStarted = World->GetTimeSeconds();

	if (TimeOut > 0.0f)
	{
		World->GetTimerManager().SetTimer(TimerHandle, this, &UAbilityTask_WaitForPredicate::OnTimeOut, TimeOut, false);
	}

	Super::Activate();
}

void UAbilityTask_WaitForPredicate::OnTimeOut()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnFinish.Broadcast();
	}

	EndTask();
}

FString UAbilityTask_WaitForPredicate::GetDebugString() const
{
	float TimeLeft = TimeOut - GetWorld()->TimeSince(TimeStarted);
	return FString::Printf(TEXT("WaitForPredicate. Time: %.2f. TimeLeft: %.2f"), TimeOut, TimeLeft);
}
