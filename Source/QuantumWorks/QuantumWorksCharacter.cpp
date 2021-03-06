// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuantumWorksCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h"

#include "QuantumWorksPlayerState.h"
#include "QwEnemyCharacter.h"





//////////////////////////////////////////////////////////////////////////
// AQuantumWorksCharacter

AQuantumWorksCharacter::AQuantumWorksCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// init the camera boom component
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeLocation(FVector(0, 0, 68.492264));

	// init the follow camera component
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(FName("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->FieldOfView = 80.0f;

	// init the follow camera component
	GunComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Gun"));

	// let ignore between cameras
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	// Makes sure that the animations play on the Server so that we can use bone and socket transforms
	// to do things like spawning projectiles and other FX.
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

}

void AQuantumWorksCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	GetCharacterMovement()->GravityScale = 1.f;		// init gravity on the floor
	bIsRunningRayVision = false;		// the x ray ability is not running at the beginning
}

//////////////////////////////////////////////////////////////////////////
// Input

void AQuantumWorksCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("TextUpdateHealth", IE_Pressed, this, &AQuantumWorksCharacter::TestUpdateHealthValue);
	PlayerInputComponent->BindKey(EKeys::E, IE_Pressed, this, &AQuantumWorksCharacter::CheckIfCanStartXRayAbility);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AQuantumWorksCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AQuantumWorksCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AQuantumWorksCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AQuantumWorksCharacter::LookUpAtRate);

	BindASCInput();		// bing the gas input
}

void AQuantumWorksCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AQuantumWorksCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AQuantumWorksCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AQuantumWorksCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AQuantumWorksCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AQuantumWorksPlayerState* PS = GetPlayerState<AQuantumWorksPlayerState>();
	if (!PS)
	{
		return;
	}

	AbilitySystemComponent = Cast<UQwAbilitySystemComponent>(PS->GetAbilitySystemComponent());
	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);			// init the ability info

	AddCharacterAbilities();	// add the ability to character
}

void AQuantumWorksCharacter::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->CharacterAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<UQwGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		// give the ability to character
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}

	AbilitySystemComponent->CharacterAbilitiesGiven = true;		// abilities can not be given twice
}

void AQuantumWorksCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	// Our controller changed, must update ActorInfo on AbilitySystemComponent
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}
}

UAbilitySystemComponent* AQuantumWorksCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void AQuantumWorksCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AQuantumWorksPlayerState* PS = GetPlayerState<AQuantumWorksPlayerState>();
	if (!PS)
	{
		return;
	}
	// get the AbilitySystemComponent from player state
	AbilitySystemComponent = Cast<UQwAbilitySystemComponent>(PS->GetAbilitySystemComponent());
	AbilitySystemComponent->InitAbilityActorInfo(PS, this);
	BindASCInput();
}

void AQuantumWorksCharacter::BindASCInput()
{
	// opertations for binding ability component input
	if (!ASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString(TEXT("ConfirmTarget")), FString("CancelTarget"), FString("EAbilityInputID")));

		ASCInputBound = true;
	}
}

void AQuantumWorksCharacter::TestUpdateHealthValue()
{

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AQwEnemyCharacter::StaticClass(), FoundActors);
	if (FoundActors.Num() == 0)
	{
		return;
	}

	for (auto OneActor : FoundActors)
	{
		AQwEnemyCharacter* EnemyCharacter = Cast<AQwEnemyCharacter>(OneActor);
		if (!EnemyCharacter)
		{
			continue;
		}

		EnemyCharacter->ReceiveDamage(444);

	}
}

void AQuantumWorksCharacter::CheckIfCanStartXRayAbility()
{
	// double press E means end the ability of x ray vision
	if (bIsRunningRayVision)
	{
		bIsRunningRayVision = false;
		return;
	}

	bIsRunningRayVision = true;
}
