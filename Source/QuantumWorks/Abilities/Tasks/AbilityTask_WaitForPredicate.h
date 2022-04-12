#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"

#include "AbilityTask_WaitForPredicate.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndedEvent);

typedef TFunction<bool(const UObject*, const float DeltaTime)> TObjectPredicate;

UCLASS()
class QUANTUMWORKS_API UAbilityTask_WaitForPredicate : public UAbilityTask
{
	GENERATED_BODY()

public:

	UAbilityTask_WaitForPredicate(const FObjectInitializer& ObjectInitializer);

	virtual void TickTask(float DeltaTime) override;
	virtual void Activate() override;
	virtual FString GetDebugString() const override;

	static UAbilityTask_WaitForPredicate* WaitForPredicate(UGameplayAbility* OwningAbility, UObject* Object, TObjectPredicate Predicate, float TimeOut = 0.0f);

private:

	void OnTimeOut();

public:

	UPROPERTY(BlueprintAssignable)
	FEndedEvent		OnFinish;

private:
	float			TimeOut;
	float			TimeStarted;
	FTimerHandle 	TimerHandle;

	UPROPERTY()
	UObject* Object;

	TObjectPredicate Predicate;
};
