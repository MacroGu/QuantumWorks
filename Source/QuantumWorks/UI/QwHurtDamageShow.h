
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QwHurtDamageShow.generated.h"


/**
 * 
 */
UCLASS()
class QUANTUMWORKS_API UQwHurtDamageShow : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayHurtDamageAnimation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (BindWidget))
	class UTextBlock* HurtDamageValue = nullptr;


};
