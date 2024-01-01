#include "Player/APPlayerCharacter.h"

#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

AAPPlayerCharacter::AAPPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	// Sets absolute rotation to TRUE so that the player controller can rotate but the camera stays facing forward.
	SpringArm->SetAbsolute(false, true, false);

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
		EnhancedInputComponent->BindAction(InputConfig->InputMove, ETriggerEvent::Triggered, this, &AAPPlayerCharacter::IC_Move_Triggered);
	}
	
	if (InputConfig->InputJump != nullptr)
	{
		EnhancedInputComponent->BindAction(InputConfig->InputJump, ETriggerEvent::Started, this, &AAPPlayerCharacter::IC_Jump_Started);
		EnhancedInputComponent->BindAction(InputConfig->InputJump, ETriggerEvent::Canceled, this, &AAPPlayerCharacter::IC_Jump_Canceled);
	}
	
	if (InputConfig->InputAttack != nullptr)
	{
		EnhancedInputComponent->BindAction(InputConfig->InputAttack, ETriggerEvent::Triggered, this, &AAPPlayerCharacter::IC_Attack_Triggered);
	}
	
	if (InputConfig->InputThrow != nullptr)
	{
		EnhancedInputComponent->BindAction(InputConfig->InputThrow, ETriggerEvent::Triggered, this, &AAPPlayerCharacter::IC_Throw_Triggered);
	}
}

void AAPPlayerCharacter::IC_Move_Triggered(const FInputActionValue& Value)
{
	if (!IsValid(MyPlayerControllerPtr.Get()))
	{
		// TODO: Logging
		return;
	}
	const FVector2D MoveValue = Value.Get<FVector2D>();

	const FRotator MovementRotation(0, MyPlayerControllerPtr->GetControlRotation().Yaw, 0);

	// /Up/Down direction
	if (MoveValue.Y != 0.0f)
	{
		const	FVector Direction = MovementRotation.RotateVector(GetActorRightVector() * -1.0);
		AddMovementInput(Direction, MoveValue.Y);
	}

	// Left/Right direction
	if (MoveValue.X != 0.0f)
	{
		//const	FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);
		//AddMovementInput(Direction, MoveValue.X);
		
		const	FVector Direction = MovementRotation.RotateVector(GetActorForwardVector());
		AddMovementInput(Direction, MoveValue.X);

		if (MoveValue.X > 0.0f)
		{
			const FRotator NewRotation = FRotator(0.0, 0.0, 0.0);
			MyPlayerControllerPtr->SetControlRotation(NewRotation);
		}
		else if (MoveValue.X < 0.0f)
		{
			const FRotator NewRotation = FRotator(0.0, 180.0, 0.0);
			MyPlayerControllerPtr->SetControlRotation(NewRotation);
		}
	}
}

void AAPPlayerCharacter::IC_Jump_Started(const FInputActionValue& Value)
{
	Jump();
};

void AAPPlayerCharacter::IC_Jump_Canceled(const FInputActionValue& Value)
{
	StopJumping();
};

void AAPPlayerCharacter::IC_Attack_Triggered(const FInputActionValue& Value)
{
	//TODO Implement
};

void AAPPlayerCharacter::IC_Throw_Triggered(const FInputActionValue& Value)
{
	//TODO Implement
};
