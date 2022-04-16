// Fill out your copyright notice in the Description page of Project Settings.


#include "QwEnemyCharacter.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

#include "QuantumWorksPlayerController.h"
#include "UI/QwHurtDamageShow.h"




// Sets default values
AQwEnemyCharacter::AQwEnemyCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	if (RootComponent == nullptr)
	{
		RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
	}

	UIHurtDamageWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("UIHurtDamageWidgetComponent"));
	UIHurtDamageWidgetComponent->SetupAttachment(RootComponent);
	UIHurtDamageWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	UIHurtDamageWidgetComponent->SetVisibility(false);

}

// Called when the game starts or when spawned
void AQwEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// InitWidgetComponent();
}

void AQwEnemyCharacter::ReceiveDamage(const int32 DamageValue)
{
	AQuantumWorksPlayerController* PC = Cast<AQuantumWorksPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PC)
	{
		return;
	}

	if (UIHurtDamageShowWidget)
	{
		UIHurtDamageShowWidget->RemoveFromParent();
	}
	UIHurtDamageShowWidget = CreateWidget<UQwHurtDamageShow>(PC, UIHurtDamageShowClass);
	UIHurtDamageShowWidget->HurtDamageValue->SetText(FText::AsNumber(DamageValue));
	UIHurtDamageWidgetComponent->SetWidget(UIHurtDamageShowWidget);
	UIHurtDamageShowWidget->PlayHurtDamageAnimation();
	UIHurtDamageWidgetComponent->SetVisibility(true);

}

void AQwEnemyCharacter::InitWidgetComponent()
{
	if (UIHurtDamageShowWidget)
	{
		return;
	}

	AQuantumWorksPlayerController* PC = Cast<AQuantumWorksPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PC)
	{
		return;
	}

	UIHurtDamageShowWidget = CreateWidget<UQwHurtDamageShow>(PC, UIHurtDamageShowClass);
	if (!UIHurtDamageShowWidget || !UIHurtDamageWidgetComponent)
	{
		return;
	}

	UIHurtDamageShowWidget->HurtDamageValue->SetText(FText::AsNumber(3333));
	UIHurtDamageWidgetComponent->SetWidget(UIHurtDamageShowWidget);


}

void AQwEnemyCharacter::UpdateHealthDamage(const int32 DamageValue)
{
	ReceiveDamage(DamageValue);
}
