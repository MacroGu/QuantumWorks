// Fill out your copyright notice in the Description page of Project Settings.


#include "QwEnemyCharacter.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

#include "QuantumWorksPlayerController.h"
#include "QwAIController.h"
#include "UI/QwHurtDamageShow.h"




// Sets default values
AQwEnemyCharacter::AQwEnemyCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (RootComponent == nullptr)
	{
		RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
	}

	UIHurtDamageWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("HurtDamage"));
	UIHurtDamageWidgetComponent->SetupAttachment(RootComponent);
	UIHurtDamageWidgetComponent->SetWidgetSpace(EWidgetSpace::World);



}

// Called when the game starts or when spawned
void AQwEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UIHurtDamageWidget = Cast<UQwHurtDamageShow>(UIHurtDamageWidgetComponent->GetUserWidgetObject());
}

void AQwEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void AQwEnemyCharacter::ReceiveDamage(const int32 DamageValue)
{
	if (!UIHurtDamageWidget)
	{
		return;
	}

	UIHurtDamageWidget->UpdateDamageHurt(DamageValue);

}
