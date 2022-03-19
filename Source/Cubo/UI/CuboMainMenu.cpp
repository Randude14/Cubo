
#include "CuboMainMenu.h"


ACuboMainMenu::ACuboMainMenu()
{
	MenuRoot = CreateDefaultSubobject<USceneComponent>("MenuRoot");
	SetRootComponent(MenuRoot);
	
	NewGame = CreateDefaultSubobject<UWidgetComponent>("NewGame");
	NewGame->SetupAttachment(MenuRoot);

	Settings = CreateDefaultSubobject<UWidgetComponent>("Settings");
	Settings->SetupAttachment(MenuRoot);

	QuitGame = CreateDefaultSubobject<UWidgetComponent>("QuitGame");
	QuitGame->SetupAttachment(MenuRoot);
}

void ACuboMainMenu::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACuboMainMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

