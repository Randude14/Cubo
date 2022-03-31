
#include "CuboMenuActor.h"

#include "CuboMenu.h"


ACuboMenuActor::ACuboMenuActor()
{
	MenuRoot = CreateDefaultSubobject<USceneComponent>("MenuRoot");
	SetRootComponent(MenuRoot);
	
	MainMenuComponent = CreateDefaultSubobject<UWidgetComponent>("MainMenu");
	MainMenuComponent->SetupAttachment(MenuRoot);
	MainMenuComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACuboMenuActor::BeginPlay()
{
	Super::BeginPlay();

	MenuScreen = Cast<UCuboMenu>( MainMenuComponent->GetUserWidgetObject() );

	if(MenuScreen)
	{
		ShowScreen("MainMenu");
		MenuScreen->OnVisibilityChanged.AddDynamic(this, &ACuboMenuActor::OnVisibilityChanged);
	}
}

void ACuboMenuActor::OnVisibilityChanged(ESlateVisibility Visibility)
{
	if(Visibility == ESlateVisibility::Hidden)
	{
		CloseScreen();
	}
}

void ACuboMenuActor::ShowScreen(FString ID)
{
	if(MenuScreen)
	{
		SetActorHiddenInGame(false);
		MainMenuComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MenuScreen->ShowWindow(ID);
	}
}

bool ACuboMenuActor::CanForceClose()
{
	if(MenuScreen)
	{
		return MenuScreen->CanForceClose();
	}
	return true;
}


void ACuboMenuActor::CloseScreen()
{
	if(MenuScreen && MenuScreen->CanForceClose())
	{
		SetActorHiddenInGame(true);
		MainMenuComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}


// Called every frame
void ACuboMenuActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

