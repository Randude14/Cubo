// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboMainMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Cubo/CuboFramework/CuboGrid.h"

#include "Components/Button.h"

void UCuboMainMenu::NativeOnInitialized()
{
	NewGameButton->OnReleased.AddDynamic(this, &UCuboMainMenu::UCuboMainMenu::NewGamePressed);
	QuitGameButton->OnReleased.AddDynamic(this, &UCuboMainMenu::QuitGamePressed);
	ResumeGameButton->OnReleased.AddDynamic(this, &UCuboMainMenu::ResumeGamePressed);
	SettingsButton->OnReleased.AddDynamic(this, &UCuboMainMenu::SettingsPressed);
	NewGameConfirmButton->OnReleased.AddDynamic(this, &UCuboMainMenu::ConfirmNewGamePressed);
	NewGameDenyButton->OnReleased.AddDynamic(this, &UCuboMainMenu::DenyNewGamePressed);

	TArray<AActor*> Grids;
	UGameplayStatics::GetAllActorsOfClass(this, ACuboGrid::StaticClass(), Grids);

	if(Grids.Num() > 0)
	{
		Grid = Cast<ACuboGrid>(Grids[0]);
	}
}

void UCuboMainMenu::NewGamePressed()
{
	
}

void UCuboMainMenu::QuitGamePressed()
{
	
}

void UCuboMainMenu::ResumeGamePressed()
{
	
}

void UCuboMainMenu::SettingsPressed()
{
	
}

void UCuboMainMenu::ConfirmNewGamePressed()
{
	
}

void UCuboMainMenu::DenyNewGamePressed()
{
	
}