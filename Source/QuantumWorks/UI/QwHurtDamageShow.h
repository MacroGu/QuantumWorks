
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
	void PlayHurtDamageAnimation();		// hurt damage UI will be shown in the non-friend character

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (BindWidget))
	class UTextBlock* HurtDamageValue = nullptr;		// hurtdamage value will show in the non-friend character


};
