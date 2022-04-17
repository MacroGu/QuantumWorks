// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "QuantumWorksPlayerController.generated.h"




class UMaterialInstanceDynamic;


/**
 * 
 */
UCLASS()
class QUANTUMWORKS_API AQuantumWorksPlayerController : public APlayerController
{
	GENERATED_BODY()
	


public:
	AQuantumWorksPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* OutlineMid;


	// configure for E ability 
	void SetNonFriendOutlineThickness(const float Thickness);
	void SetOthersOutlineThickness(const float Thickness);

protected:

	// Server only
	virtual void OnPossess(APawn* InPawn) override;


};
