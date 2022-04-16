// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "QwEnemyCharacter.generated.h"



class UQwHurtDamageShow;




UCLASS()
class QUANTUMWORKS_API AQwEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AQwEnemyCharacter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UWidgetComponent* UIHurtDamageWidgetComponent;

	void ReceiveDamage(const int32 DamageValue);


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ExWorld|UI")
	TSubclassOf<UQwHurtDamageShow> UIHurtDamageShowClass;

	UPROPERTY(BlueprintReadWrite)
	UQwHurtDamageShow* UIHurtDamageShowWidget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	UQwHurtDamageShow* UIHurtDamageWidget;




public:	
	
	UPROPERTY(EditAnywhere)
	FGameplayTag EntityTag;

};
