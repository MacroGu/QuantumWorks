// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "QwAIController.generated.h"

/**
 * 
 */
UCLASS()
class QUANTUMWORKS_API AQwAIController : public AAIController
{
	GENERATED_BODY()
	

public:
	AQwAIController();

	void ShowHurtDamage(const int32 HurtValue);

};
