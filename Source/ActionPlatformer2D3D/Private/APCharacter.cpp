#include "APCharacter.h"

AAPCharacter::AAPCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	CombatComponent = CreateDefaultSubobject<UAPCombatComponent>(TEXT("Combat Component"));
}