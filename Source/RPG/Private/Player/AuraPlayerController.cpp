// Copyright DynamicVenki


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext,0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Type::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CurosrHit;
	GetHitResultUnderCursor(ECC_Visibility,false,CurosrHit);
	if (!CurosrHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CurosrHit.GetActor();
	
	/*
	 *	LineTrace from cursor. There are several scenarios.
	 *	A. LastActor is null && ThisActor is null
	 *		-Do Nothing
	 *	B. LastActor is null && ThisActor is valid
	 *		-Highlight ThisActor
	 *	C. LastActor is valid && ThisActor is null
	 *		-UnHighlight LastActor
	 *	D. Both actors are valid && LastActor != ThisActor
	 *		-UnHighlight LastActor , and Highlight ThisActor
	 *	E. Both actors are valid , and are the same actor
	 *		-Do nothing
	 */

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// Case B
			ThisActor->HighlightActor();
		}
		if (ThisActor == nullptr)
		{
			// Case A - both are null , do nothing
		}
	}
	else  // LastActor is valid
	{
		if (ThisActor == nullptr)
		{
			//Case C
			LastActor->UnHightlightActor();
		}
		else //Both Actors are Valid
		{
			if (LastActor != ThisActor)
			{
				//Case D
				LastActor->UnHightlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				//Case E - Do nothing
			}
		}
	}
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const struct FInputActionValue& InputActionValue)
{
	
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.0f,Rotation.Yaw,0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
	}
	
}

