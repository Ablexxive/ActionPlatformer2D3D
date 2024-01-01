#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"

#include "APInputConfig.generated.h"

UCLASS()
class UAPInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputMove;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputJump;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAttack;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputThrow;
};