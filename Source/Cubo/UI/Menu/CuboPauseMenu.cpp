#include "CuboPauseMenu.h"
#include "Components/Button.h"
#include "Cubo/CuboFramework/CuboPlayerController.h"

void UCuboPauseMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if(ResumeButton)
	{
		ResumeButton->OnReleased.AddDynamic(this, &UCuboPauseMenu::ResumeButtonPressed);
	}
	bCanForceClose = true;
}

void UCuboPauseMenu::ResumeButtonPressed()
{
	if(ACuboPlayerController* PlayerController = Cast<ACuboPlayerController>( GetOwningPlayer() ))
	{
		if(ACuboGrid* Grid = Cast<ACuboGrid>(PlayerController->GetOwningGrid()))
		{
			Grid->Resume();
		}
	}
	WindowReturn.Broadcast();
}
