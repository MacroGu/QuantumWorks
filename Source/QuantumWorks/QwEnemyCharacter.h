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
	class UWidgetComponent* UIHurtDamageWidgetComponent;	// hurt damage ui component

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UI")
	TSubclassOf<UQwHurtDamageShow> UIHurtDamageShowClass;		// configure blueprint hurt damagae ui

	UPROPERTY(BlueprintReadWrite)
	UQwHurtDamageShow* UIHurtDamageShowWidget;		// hurt damage UI

	// opeartions for receiving damage
	void ReceiveDamage(const int32 DamageValue);

public:	
	
	UPROPERTY(EditAnywhere)
	FGameplayTag EntityTag;		// tag for non friend or friend

};
