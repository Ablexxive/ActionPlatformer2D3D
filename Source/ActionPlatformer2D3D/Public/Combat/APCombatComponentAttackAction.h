#pragma once

#include "CoreMinimal.h"
#include "APCombatComponent.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "APCombatComponentAttackAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAPCombatComponentDoAttackOutputSignature);

UCLASS()
class UAPCombatComponentAttackAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	TWeakObjectPtr<UAPCombatComponent> AttackerCombatComponentPtr;

	UPROPERTY(BlueprintAssignable)
	FAPCombatComponentDoAttackOutputSignature OnCompleted;
	
	UPROPERTY(BlueprintAssignable)
	FAPCombatComponentDoAttackOutputSignature OnCancelled;

public:
	UFUNCTION(BlueprintCallable, Category = "Combat", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"))
	static UAPCombatComponentAttackAction* DoAttackWithCallbacks(UAPCombatComponent* AttackerCombatComponent, UObject* WorldContextObject);

	virtual void Activate() override;

private:
	UFUNCTION()
	void OnAttackEnd(bool bCompleted);
};