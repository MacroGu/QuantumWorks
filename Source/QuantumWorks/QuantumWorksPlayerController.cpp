// Fill out your copyright notice in the Description page of Project Settings.


#include "QuantumWorksPlayerController.h"

#include "Materials/MaterialInstanceDynamic.h"

#include "QuantumWorksPlayerState.h"
#include "Abilities/QwAbilitySystemComponent.h"


AQuantumWorksPlayerController::AQuantumWorksPlayerController()
{

}

// Server only
void AQuantumWorksPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AQuantumWorksPlayerState* PS = GetPlayerState<AQuantumWorksPlayerState>();
	if (PS)
	{
		// Init ASC with PS (Owner) and our new Pawn (AvatarActor)
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}


void AQuantumWorksPlayerController::SetNonFriendOutlineThickness(const float Thickness)
{
	if (!OutlineMid)
	{
		return;
	}

	OutlineMid->SetScalarParameterValue(TEXT("Outline Width Not Friend"), Thickness);

}

void AQuantumWorksPlayerController::SetOthersOutlineThickness(const float Thickness)
{
	if (!OutlineMid)
	{
		return;
	}

	OutlineMid->SetScalarParameterValue(TEXT("Outline Width Others"), Thickness);
}
