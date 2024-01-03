#pragma once

#include "CoreMinimal.h"

#include "PaperZDCharacter.h"
#include "APCharacter.generated.h"

//TODO Figure out log categories?? For next project maybe?

UCLASS()
class  AAPCharacter : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	AAPCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAttacking = false;

	// What will be in here:
	// - Hitbox system??
	// - Stun System
	// - Damage system
};