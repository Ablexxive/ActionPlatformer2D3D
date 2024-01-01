#pragma once

#include "CoreMinimal.h"

#include "APCharacter.h"
#include "Input/APInputConfig.h"
#include "APPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;

UCLASS()
class AAPPlayerCharacter : public AAPCharacter
{
	GENERATED_BODY()

public:
	AAPPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> Camera;

#pragma region // input
private:
	/* Maps key inputs to input actions defined in InputConfig */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* InputMapping;

	/*	My datastruct that defines Move/Jump/Ect. */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UAPInputConfig* InputConfig;

	TWeakObjectPtr<APlayerController> MyPlayerControllerPtr; 

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/* Input Config functions. */
	void IC_Move_Triggered(const FInputActionValue& Value);
	void IC_Jump_Started(const FInputActionValue& Value);
	void IC_Jump_Canceled(const FInputActionValue& Value);
	void IC_Attack_Triggered(const FInputActionValue& Value);
	void IC_Throw_Triggered(const FInputActionValue& Value);
#pragma endregion 

	
};