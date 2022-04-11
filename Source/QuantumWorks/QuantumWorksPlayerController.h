// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "QuantumWorksPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class QUANTUMWORKS_API AQuantumWorksPlayerController : public APlayerController
{
	GENERATED_BODY()
	


public:
	AQuantumWorksPlayerController();



protected:

	// Server only
	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_PlayerState() override;

};
