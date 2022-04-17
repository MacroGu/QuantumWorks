// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "QuantumWorksPlayerState.generated.h"



class UQwAbilitySystemComponent;


/**
 * 
 */
UCLASS()
class QUANTUMWORKS_API AQuantumWorksPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	

public:
	AQuantumWorksPlayerState();

	// Implement IAbilitySystemInterface
	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY()
	UQwAbilitySystemComponent* AbilitySystemComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
