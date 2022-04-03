#include "CuboConfirmNewGame.h"
#include "Cubo/CuboFramework/CuboPlayerController.h"
#include "Cubo/CuboFramework/CuboGrid.h"

#include "Components/Button.h"

void UCuboConfirmNewGame::NativeOnInitialized()
{
	if(NewGameConfirmButton)
	{
		NewGameConfirmButton->OnReleased.AddDynamic(this, &UCuboConfirmNewGame::ConfirmPressed);
	}
	if(NewGameDenyButton)
	{
		NewGameDenyButton->OnReleased.AddDynamic(this, &UCuboConfirmNewGame::DenyPressed);
	}
	bCanForceClose = true;
}

void UCuboConfirmNewGame::ConfirmPressed()
{
	if(ACuboPlayerController* PlayerController = Cast<ACuboPlayerController>(GetOwningPlayer()))
	{
		if(ACuboGrid* Grid = PlayerController->GetOwningGrid())
		{
			Grid->NewGame();
			WindowReturn.Broadcast();
			WindowReturn.Broadcast();
		}
	}
}

void UCuboConfirmNewGame::DenyPressed()
{
	WindowReturn.Broadcast();
}


