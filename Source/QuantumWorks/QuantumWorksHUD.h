// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "QuantumWorksHUD.generated.h"

UCLASS()
class AQuantumWorksHUD : public AHUD
{
	GENERATED_BODY()

public:
	AQuantumWorksHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

