// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "QuantumWorksPlayerState.generated.h"



class UQwAttributeSetBase;
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

	UQwAttributeSetBase* GetAttributeSetBase() const;


	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowAbilityConfirmCancelText(bool ShowText);


	/**
	* Getters for attributes from GDAttributeSetBase. Returns Current Value unless otherwise specified.
	*/
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxHealth() const;

protected:
	UPROPERTY()
	UQwAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UQwAttributeSetBase* AttributeSetBase;

	FGameplayTag DeadTag;

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);



};