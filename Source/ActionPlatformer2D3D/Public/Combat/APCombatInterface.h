#pragma once

#include "CoreMinimal.h"
#include "APCombatComponent.h"
#include "UObject/Interface.h"

#include "APCombatInterface.generated.h"

//UINTERFACE(BlueprintType, NotBlueprintable)
UINTERFACE(BlueprintType, MinimalAPI, Category = "Combat", meta = (CannotImplementInterfaceInBlueprint))
class UAPCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class IAPCombatInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual UAPCombatComponent* GetCombatComponent() const = 0;
	
	//UFUNCTION(BlueprintCallable, Category = "Inventory")
	//virtual UTOWItemInventoryComponent* GetTOWItemInventory() const = 0;
	/*
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Combat")
	void ToggleAttackHitbox(bool enabled);
	*/

};