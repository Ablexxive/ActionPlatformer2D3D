// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/APCombatComponent.h"

#include "Combat/APCombatInterface.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundCue.h"
#include "PaperZDCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/APPlayerCharacter.h"

UAPCombatComponent::UAPCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UAPCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Find the attack hit box - UBoxComponent
	const AActor* MyOwner = GetOwner();
	TSet<UActorComponent*> AllComponents = MyOwner->GetComponents();
	for (UActorComponent* ComponentPtr : AllComponents)
	{
		if (ComponentPtr == nullptr)
		{
			continue;	
		}
		
		UBoxComponent* BoxComponent = Cast<UBoxComponent>(ComponentPtr);
		if (BoxComponent == nullptr)
		{
			continue;
		}

		if (BoxComponent->GetName() == AttackHitboxName)
		{
			AttackHitBoxPtr = BoxComponent;
			FoundAttackHitBox++;
		}
	}

	// TODO Log error?
	//if (FoundAttackHitBox>1)
	
	// Bind on overlap
	UBoxComponent* HitBox = AttackHitBoxPtr.Get();
	if (HitBox != nullptr)
	{
		HitBox->OnComponentBeginOverlap.AddDynamic(this, &UAPCombatComponent::BeginAttackHitboxOverlap);
		HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Bind attack animation complete
	AttackAnimationOverideDelegate.BindUObject(this, &UAPCombatComponent::AttackAnimationComplete);

	// Save weakptr to AnimInstance
	if (const APaperZDCharacter* MyOwnerZD = Cast<APaperZDCharacter>(MyOwner))
	{
		AnimInstancePtr = MyOwnerZD->GetAnimInstance();	
	}

	// Save off relative location for shaking animation
	if (const APaperZDCharacter* Owner = Cast<APaperZDCharacter>(GetOwner()))
	{
		if (const UPaperFlipbookComponent* MySprite = Owner->GetSprite())
		{
			SpriteDefaultRelativeLocation = MySprite->GetRelativeLocation();
		}
	}
}

void UAPCombatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	AttackAnimationOverideDelegate.Unbind();
}

void UAPCombatComponent::ToggleAttackHitbox(bool Enabled)
{
	UBoxComponent* HitBox = AttackHitBoxPtr.Get();
	if (HitBox != nullptr)
	{
		if (Enabled)
		{
			HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
		else
		{
			HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	
}

void UAPCombatComponent::TakeDamage(uint8 InDamage)
{
	CurrentHealth -= InDamage;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);

	PlayHitStunSound();

	if (CurrentHealth <= 0)
	{
		OnCombatActorDefeated.Broadcast();
		IsDead = true;
		
		if (UPaperZDAnimInstance* AnimInstance = AnimInstancePtr.Get())
		{
			AnimInstance->JumpToNode(ABPJumpName_Dead);
		}

		// TODO This should prob be handled by the character?
		if (const APaperZDCharacter* MyOwner = Cast<APaperZDCharacter>(GetOwner()))
		{
			if (UCapsuleComponent* Capsule = MyOwner->GetCapsuleComponent())
			{
				Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}
	else
	{
		BeginHitStun();
		BeginHitPause();
	}
}

void UAPCombatComponent::DoAttack()
{
	if (IsAttacking)
	{
		return;
	}
	
	if (UPaperZDAnimInstance* AnimInstance = AnimInstancePtr.Get())
	{
		AnimInstance->PlayAnimationOverride(AttackAnimSequence, AttackAnimSequenceSlot, 1, 0, AttackAnimationOverideDelegate);
		IsAttacking = true;
	}
}

void UAPCombatComponent::AttackAnimationComplete(bool Success)
{
	// If (success) -> Completed, else Canceled.
	IsAttacking = false;
}

void UAPCombatComponent::BeginAttackHitboxOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* Other,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (IAPCombatInterface* OtherCombatInterface = Cast<IAPCombatInterface>(Other))
	{
		if (UAPCombatComponent* OtherCombatComponent  = OtherCombatInterface->GetCombatComponent())
		{
			if (MyFaction != OtherCombatComponent->MyFaction)
			{
				// Do damage to the other combat component.
				OtherCombatComponent->TakeDamage(AttackDamage);

				BeginHitPause();
			}
		}
	}
}

void UAPCombatComponent::BeginHitStun()
{
	if (const UWorld* World = GetWorld())
	{
		FTimerHandle StunTimerHandle; // TODO Maybe keep this ref in .h?
		World->GetTimerManager().SetTimer(
			StunTimerHandle,
			this,
			&UAPCombatComponent::EndHitStun,
			StunDuration,
			false);
		
		IsStunned = true;
		OnStunStatusChanged.Broadcast(IsStunned);

		// Begin Stun Animation Timer
		World->GetTimerManager().SetTimer(
			StunAnimationTimerHandle,
			this,
			&UAPCombatComponent::StunAnimation,
			StunAnimationLoop,
			true);
	}
	
	if (UPaperZDAnimInstance* AnimInstance = AnimInstancePtr.Get())
	{
		AnimInstance->JumpToNode(ABPJumpName_HitStun);
	}
}

void UAPCombatComponent::EndHitStun()
{
	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(StunAnimationTimerHandle);
	}
	// Reset default sprite relative location after animation.
	if (const APaperZDCharacter* Owner = Cast<APaperZDCharacter>(GetOwner()))
	{
		if (UPaperFlipbookComponent* MySprite = Owner->GetSprite())
		{
			MySprite->SetRelativeLocation(SpriteDefaultRelativeLocation);			
		}
	}
	
	IsStunned = false;
	OnStunStatusChanged.Broadcast(IsStunned);
}

void UAPCombatComponent::StunAnimation()
{
	// TODO Should this just live on the owner and respond to a broadcast from being hit? Prob.
	if (const APaperZDCharacter* Owner = Cast<APaperZDCharacter>(GetOwner()))
	{
		if (UPaperFlipbookComponent* MySprite = Owner->GetSprite())
		{
			StunAnimation_flipflop = !StunAnimation_flipflop;
			const float x_offset = StunAnimation_flipflop ? StunAnimationTranslation : StunAnimationTranslation * -1.0; 
			const FVector NewRelLocation = SpriteDefaultRelativeLocation + FVector(x_offset, 0, 0);
			MySprite->SetRelativeLocation(NewRelLocation);			
		}
	}
}

void UAPCombatComponent::PlayHitStunSound() const
{
	if (HitStunSoundCue)
	{
		if (const UWorld* World = GetWorld())
		{
			UGameplayStatics::PlaySoundAtLocation(
				World,
				HitStunSoundCue,
				GetOwner()->GetActorLocation(),
				GetOwner()->GetActorRotation()
			);	
		}
	}
}


void UAPCombatComponent::BeginHitPause()
{
	if (AActor* Owner = GetOwner())
	{
		Owner->CustomTimeDilation = 0.0;
	}
	
	if (const UWorld* World = GetWorld())
	{
		FTimerHandle HitPauseTimerHandle; // TODO Maybe keep this ref in .h?
		World->GetTimerManager().SetTimer(
			HitPauseTimerHandle,
			this,
			&UAPCombatComponent::EndHitPause,
			HitPauseDuration,
			false);
	}
	
	IsHitPaused = true;
}

void UAPCombatComponent::EndHitPause()
{
	if (AActor* Owner = GetOwner())
	{
		Owner->CustomTimeDilation = 1.0;
	}
	IsHitPaused = false;
}
