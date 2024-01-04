// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDAnimInstance.h"
#include "Components/ActorComponent.h"
#include "APCombatComponent.generated.h"

UENUM(BlueprintType)
enum class EFaction : uint8
{
	Player = 0	UMETA(DisplayName = "Player"),
	Enemy = 1	UMETA(DisplayName = "Enemy")
};

class UBoxComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONPLATFORMER2D3D_API UAPCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAPCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// TODO - can probably remove the tick stuff
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat|Attack")
	uint8 AttackDamage = 30;
	
	UPROPERTY(EditAnywhere, Category = "Combat|Attack")
	FName AttackHitboxName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Attack")
	UPaperZDAnimSequence* AttackAnimSequence;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Attack")
	FName AttackAnimSequenceSlot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat|Attack")
	bool IsAttacking = false;

	// DEBUG REMOVE
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat|Attack")
	uint8 FoundAttackHitBox = 0;

	UFUNCTION(BlueprintCallable, Category="Combat|Attack")
	void DoAttack(UPaperZDAnimInstance* AnimInstance);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat|Character")
	uint8 MaxHealth = 100;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Combat|Character")
	uint8 CurrentHealth = 100;

	UPROPERTY(EditAnywhere, Category = "Combat|Character")
	EFaction MyFaction = EFaction::Player;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ToggleAttackHitbox(bool Enabled);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void TakeDamage(uint8 InDamage);
	
private:
	TWeakObjectPtr<UBoxComponent> AttackHitBoxPtr;
	
	FZDOnAnimationOverrideEndSignature AttackAnimationOverideDelegate;
	void AttackAnimationComplete(bool Success);

	UFUNCTION()
	void BeginAttackHitboxOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* Other,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};
