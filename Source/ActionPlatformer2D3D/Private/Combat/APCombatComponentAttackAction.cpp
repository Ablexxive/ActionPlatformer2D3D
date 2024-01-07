#include "Combat/APCombatComponentAttackAction.h"
#include "UObject/Stack.h"

UAPCombatComponentAttackAction* UAPCombatComponentAttackAction::DoAttackWithCallbacks(
	UAPCombatComponent* AttackerCombatComponent,
	UObject* WorldContextObject)
{
	UAPCombatComponentAttackAction* Node = NewObject<UAPCombatComponentAttackAction>();
	Node->AttackerCombatComponentPtr = AttackerCombatComponent;
	Node->RegisterWithGameInstance(WorldContextObject);
	return Node;
}

void UAPCombatComponentAttackAction::Activate()
{
	Super::Activate();
	if (UAPCombatComponent* AttackerCombatComp = AttackerCombatComponentPtr.Get())
	{
		FAPOnAttackEndSignature& OnEndDelegate = AttackerCombatComp->OnAttackEndSignature;
		OnEndDelegate.BindUObject(this, &UAPCombatComponentAttackAction::OnAttackEnd);
		AttackerCombatComponentPtr->DoAttack();
	}
	else
	{
		// For some reason the combat component is no longer valid	
		SetReadyToDestroy();
	}
}

void UAPCombatComponentAttackAction::OnAttackEnd(bool bCompleted)
{
	if (bCompleted)
	{
		OnCompleted.Broadcast();
	}
	else
	{
		OnCancelled.Broadcast();
	}

	SetReadyToDestroy();
}
