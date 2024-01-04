// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/APCombatComponent.h"

#include "DiffUtils.h"
#include "Combat/APCombatInterface.h"
#include "Components/BoxComponent.h"

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
	// TODO Check 0, broadcast playing dying, broadcast on hit for hitstun?
	// New interface for changing animations?
	CurrentHealth -= InDamage;
}

void UAPCombatComponent::AttackAnimationComplete(bool Success)
{
	// If (success) -> Completed, else Canceled.
	IsAttacking = false;
}

void UAPCombatComponent::DoAttack(UPaperZDAnimInstance* AnimInstance)
{
	if (AnimInstance)
	{
		AnimInstance->PlayAnimationOverride(AttackAnimSequence, AttackAnimSequenceSlot, 1, 0, AttackAnimationOverideDelegate);
		IsAttacking = true;
	}
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
	//if (OtherCombatInterface)
		
	// Here we:
	// 1) Get Combat Component - make fcn on Interface
	// 2) Check Faction
	// 3) Apply Damage - make function in C++ -> TakeDamage
}

// Called every frame
void UAPCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}