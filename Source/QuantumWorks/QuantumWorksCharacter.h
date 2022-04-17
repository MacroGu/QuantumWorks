// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/QwAbilitySystemComponent.h"
#include "QuantumWorksCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;
class USpringArmComponent;


UCLASS(config=Game)
class AQuantumWorksCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()


	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;		// camera  spring

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;			// follow camera

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Gun", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* GunComponent;		// the gun component



public:
	bool GetIsRunningXRayVision() const { return bIsRunningRayVision; }		// get if the x ray ability is running

public:
	AQuantumWorksCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Controller() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;	// Implement IAbilitySystemInterface


protected:
	virtual void BeginPlay();
	// Client only
	virtual void OnRep_PlayerState() override;

	void BindASCInput();
	virtual void AddCharacterAbilities();
	bool bIsRunningRayVision;	// to show if the ability is running


protected:
	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ExWorldTest|Abilities")
	TArray<TSubclassOf<class UQwGameplayAbility>> CharacterAbilities;

	// ability component
	TWeakObjectPtr<class UQwAbilitySystemComponent> AbilitySystemComponent;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint8 bUsingMotionControllers : 1;

	// let the ability component input binding only once
	bool ASCInputBound = false;

protected:
	
	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	// just test function for update health 
	void TestUpdateHealthValue();

	// check if the x ray ability is running
	void CheckIfCanStartXRayAbility();
};

