// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboMainMenu.h"

#include "Components/Button.h"
#include "Cubo/CuboFramework/CuboPlayerController.h"


void UCuboMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if(NewGameButton)
	{
		NewGameButton->OnReleased.AddDynamic(this, &UCuboMainMenu::NewGamePressed);
	}
	if(QuitGameButton)
	{
		QuitGameButton->OnReleased.AddDynamic(this, &UCuboMainMenu::QuitGamePressed);
	}
	if(SettingsButton)
	{
		SettingsButton->OnReleased.AddDynamic(this, &UCuboMainMenu::SettingsPressed);
	}
}

void UCuboMainMenu::NativeDestruct()
{
	Super::NativeDestruct();
}


void UCuboMainMenu::NewGamePressed()
{
	if(ACuboPlayerController* PlayerController = Cast<ACuboPlayerController>(GetOwningPlayer()))
	{
		if(ACuboGrid* Grid = PlayerController->GetOwningGrid())
		{
			if(Grid->IsGameOver())
			{
				Grid->NewGame();
				WindowReturn.Broadcast();
			}
			else
			{
				WindowOpenRequest.Broadcast("ConfirmNewGameWindow");
			}
		}
	}
}

void UCuboMainMenu::QuitGamePressed()
{
	if(GetOwningPlayer())
	{
		GetOwningPlayer()->ConsoleCommand("quit");
	}
}

void UCuboMainMenu::SettingsPressed()
{
	WindowOpenRequest.Broadcast("Settings");
}