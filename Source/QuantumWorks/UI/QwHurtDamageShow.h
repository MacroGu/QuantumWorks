
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
	void UpdateDamageHurt(int32 HurtDamage);

};
