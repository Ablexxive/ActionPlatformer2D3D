#pragma once

#include "CoreMinimal.h"

#include "APCharacter.h"
#include "Input/APInputConfig.h"
#include "PaperZDAnimInstance.h"
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

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> Camera;

	// TODO Move attack things to an attack component? Can move IsAttacking there too.
	FZDOnAnimationOverrideEndSignature AttackAnimationOverideDelegate;
	void AttackAnimationComplete(bool Success);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attacks")
	UPaperZDAnimSequence* AttackAnimSequence;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attacks")
	FName AttackAnimSequenceSlot;
	

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