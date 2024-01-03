#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "APCombatInterface.generated.h"

UINTERFACE(BlueprintType)
class UAPCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class IAPCombatInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Combat")
	void ToggleAttackHitbox(bool enabled);
};