// Copyright DynamicVenki


#include "Character/AuraEnemy.h"

void AAuraEnemy::HighlightActor()
{
	bHighlighted = true;
	GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Green,"Highlight Actor");
}

void AAuraEnemy::UnHightlightActor()
{
	bHighlighted = false;
	GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Red,"UnHighlight Actor");
}
