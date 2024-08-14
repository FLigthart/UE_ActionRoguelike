#include "SPlayerController.h"

#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"

void ASPlayerController::TogglePauseMenu()
{
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())	// Pause Menu is visible, remove it.
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		bShowMouseCursor =  false;
		SetInputMode(FInputModeGameOnly());
		return;
	}

	// Pause menu is not visible, add it.
	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuInstance)	
	{
		PauseMenuInstance->AddToViewport(100);

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}

void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	TObjectPtr<UEnhancedInputComponent> InputComp = Cast<UEnhancedInputComponent>(InputComponent);

	if (ensureAlways(InputComp))
	{
		InputComp->BindAction(Input_PauseMenu, ETriggerEvent::Started, this, &ASPlayerController::TogglePauseMenu);
	}
}
