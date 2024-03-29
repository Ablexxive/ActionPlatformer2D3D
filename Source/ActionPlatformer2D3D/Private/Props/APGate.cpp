#include "Props/APGate.h"

AAPGate::AAPGate()
{
	
}

bool AAPGate::RequirementsMet(int InEnemies, int InTreasure) const
{
	return (InEnemies == Enemies && InTreasure == Treasure);
}
