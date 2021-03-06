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

	// create widget component
	UIHurtDamageWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(FName("UIHurtDamageWidgetComponent"));
	UIHurtDamageWidgetComponent->SetupAttachment(RootComponent);
	UIHurtDamageWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	UIHurtDamageWidgetComponent->SetVisibility(false);

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
		UIHurtDamageShowWidget->RemoveFromParent();		// remove old ui
	}

	UIHurtDamageShowWidget = CreateWidget<UQwHurtDamageShow>(PC, UIHurtDamageShowClass);		// create new hurt damage ui 
	UIHurtDamageShowWidget->HurtDamageValue->SetText(FText::AsNumber(DamageValue));		// set damage value
	UIHurtDamageWidgetComponent->SetWidget(UIHurtDamageShowWidget);
	UIHurtDamageShowWidget->PlayHurtDamageAnimation();		// play ui animation
	UIHurtDamageWidgetComponent->SetVisibility(true);		// set the ui visible

}
