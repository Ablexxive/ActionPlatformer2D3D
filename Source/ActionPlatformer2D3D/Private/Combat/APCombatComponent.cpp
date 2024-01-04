// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/APCombatComponent.h"

#include "Combat/APCombatInterface.h"
#include "Components/BoxComponent.h"
#include "PaperZDCharacter.h"

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
	UPaperZDAnimInstance* AnimInstance = AnimInstancePtr.Get();
	if (AnimInstance == nullptr)
	{
		// TODO Logging
		return;
	}
	
	if (CurrentHealth <= 0)
	{
		// TODO Check 0, broadcast playing dying?
		AnimInstance->JumpToNode(ABPJumpName_Dead);
		IsDead = true;
	}
	else
	{
	   FTimerHandle StunTimerHandle; // TODO Maybe keep this ref in .h?
	   GetWorld()->GetTimerManager().SetTimer(StunTimerHandle, this, &UAPCombatComponent::_StunTimerComplete, StunDuration, false);
	
		AnimInstance->JumpToNode(ABPJumpName_HitReact);
		IsStunned = true;
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
			}
		}
	}
}

void UAPCombatComponent::_StunTimerComplete()
{
	IsStunned = false;
}

void UAPCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}