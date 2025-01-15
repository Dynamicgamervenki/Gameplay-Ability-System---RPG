// Copyright DynamicVenki

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
public :
	AAuraPlayerController();
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere,Category=Input)
	TObjectPtr<class UInputMappingContext> AuraContext;	
};
