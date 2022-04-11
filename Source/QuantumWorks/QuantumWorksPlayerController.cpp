// Fill out your copyright notice in the Description page of Project Settings.


#include "QuantumWorksPlayerController.h"


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

void AQuantumWorksPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

}