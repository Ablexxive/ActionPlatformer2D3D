#pragma once

#include "CoreMinimal.h"

#include "APGate.generated.h"

UCLASS()
class ACTIONPLATFORMER2D3D_API AAPGate : public AActor
{
	GENERATED_BODY()

public:
	AAPGate();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Enemies = 0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Treasure = 0;

	UFUNCTION(BlueprintCallable)
	bool RequirementsMet(int InEnemies, int InTreasure) const;
};