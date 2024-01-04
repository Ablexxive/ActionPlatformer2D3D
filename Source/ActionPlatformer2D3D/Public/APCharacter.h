#pragma once

#include "CoreMinimal.h"

#include "PaperZDCharacter.h"
#include "Combat/APCombatInterface.h"
#include "APCharacter.generated.h"

//TODO Figure out log categories?? For next project maybe?

// If our game was to have NPCs, I'd put the IAPCombatInterface on a APCombatCharacter class maybe?
UCLASS()
class  AAPCharacter : public APaperZDCharacter, public IAPCombatInterface
{
	GENERATED_BODY()

public:
	AAPCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//UFUNCTION()
	virtual UAPCombatComponent* GetCombatComponent() const override { return CombatComponent; }

	// You need to add UPROPERTY for TObjectPtrs so that the thing is known to unreal GC
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UAPCombatComponent> CombatComponent;
	
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAttacking = false;
	UFUNCTION()
	virtual EFaction GetFaction_Implementation() const override { return MyFaction; }

protected:
	UPROPERTY(EditAnywhere, Category = "Combat")
	EFaction MyFaction = EFaction::Player;
	*/
};