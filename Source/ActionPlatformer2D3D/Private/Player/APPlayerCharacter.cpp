#include "Player/APPlayerCharacter.h"

//#include "Engine/LocalPlayer.h"

#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PawnMovementComponent.h"

AAPPlayerCharacter::AAPPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void AAPPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// TODO This might be better in Begin Play?
	MyPlayerControllerPtr  = Cast<APlayerController>(GetController());

	if (!IsValid(MyPlayerControllerPtr.Get()))
	{
		// TODO: Add Logging
		return;
	}
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(MyPlayerControllerPtr->GetLocalPlayer());

	InputSubsystem->ClearAllMappings();
	if (InputMapping == nullptr)
	{
		// TODO: Add Logging
		return;
	}
	InputSubsystem->AddMappingContext(InputMapping, 0);

	// TODO: Is this okay here or in BeginPlay?
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent == nullptr)
	{
		// TODO: Add Logging
		return;
	}

	// TODO - Should I be keeping references to these bindings and then explicitly unbind them?
	if (InputConfig == nullptr)
	{
		// TODO: Add Logging
		return;	
	}
	
	if (InputConfig->InputMove != nullptr)
	{
		EnhancedInputComponent->BindAction(InputConfig->InputMove, ETriggerEvent::Triggered, this, &AAPPlayerCharacter::IC_Move);
	}
	/*
	if (InputConfig->KeyboardInputMove_X != nullptr)
	{
		EnhancedInputComponent->BindAction(InputConfig->KeyboardInputMove_X, ETriggerEvent::Triggered, this, &AAPPlayerCharacter::IC_KeyboardMove_X);
	}
	if (InputConfig->KeyboardInputMove_Y != nullptr)
	{
		EnhancedInputComponent->BindAction(InputConfig->KeyboardInputMove_Y, ETriggerEvent::Triggered, this, &AAPPlayerCharacter::IC_KeyboardMove_Y);
	}
	*/
	if (InputConfig->InputJump != nullptr)
	{
		EnhancedInputComponent->BindAction(InputConfig->InputJump, ETriggerEvent::Triggered, this, &AAPPlayerCharacter::IC_Jump);
	}
	if (InputConfig->InputAttack != nullptr)
	{
		EnhancedInputComponent->BindAction(InputConfig->InputAttack, ETriggerEvent::Triggered, this, &AAPPlayerCharacter::IC_Attack);
	}
}

void AAPPlayerCharacter::IC_Move(const FInputActionValue& Value)
{
	if (!IsValid(MyPlayerControllerPtr.Get()))
	{
		// TODO: Logging
		return;
	}
	const FVector2D MoveValue = Value.Get<FVector2D>();

	const FRotator MovementRotation(0, MyPlayerControllerPtr->GetControlRotation().Yaw, 0);

	// Forward/Back direction
	if (MoveValue.Y != 0.0f)
	{
		const	FVector Direction = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(Direction, MoveValue.Y);
	}

	// Left/Right direction?
	if (MoveValue.X != 0.0f)
	{
		const	FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(Direction, MoveValue.X);
	}
}

/*
void AAPPlayerCharacter::IC_KeyboardMove_X(const FInputActionValue& Value)
{
	if (!IsValid(MyPlayerControllerPtr.Get()))
	{
		// TODO: Logging
		return;
	}
	const float MoveValue = Value.Get<float>();

	const FRotator MovementRotation(0, MyPlayerControllerPtr->GetControlRotation().Yaw, 0);

	// Forward/Back direction
	if (MoveValue != 0.0f)
	{
		const	FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(Direction, MoveValue);
	}
}

void AAPPlayerCharacter::IC_KeyboardMove_Y(const FInputActionValue& Value)
{
	//TODO abstract to FowardVector vs RightVector
};
*/
void AAPPlayerCharacter::IC_Jump(const FInputActionValue& Value)
{
	Jump();
	//TODO Implement
};

void AAPPlayerCharacter::IC_Attack(const FInputActionValue& Value)
{
	//TODO Implement
};
