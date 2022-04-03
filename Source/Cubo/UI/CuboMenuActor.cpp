
#include "CuboMenuActor.h"

#include "CuboMenu.h"

// Define custom channels
#define COLLISION_MainMenu		ECC_GameTraceChannel1

ACuboMenuActor::ACuboMenuActor()
{
	MenuRoot = CreateDefaultSubobject<USceneComponent>("MenuRoot");
	SetRootComponent(MenuRoot);
	
	MainMenuComponent = CreateDefaultSubobject<UWidgetComponent>("MainMenu");
	MainMenuComponent->SetupAttachment(MenuRoot);
	MainMenuComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ACuboMenuActor::BeginPlay()
{
	Super::BeginPlay();

	MenuScreen = Cast<UCuboMenu>( MainMenuComponent->GetUserWidgetObject() );

	if(MenuScreen)
	{
		MainMenuComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		MenuScreen->OnVisibilityChanged.AddDynamic(this, &ACuboMenuActor::OnVisibilityChanged);
		ShowScreen("MainMenu");
	}
}

void ACuboMenuActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if(MenuScreen)
	{
		MenuScreen->OnVisibilityChanged.RemoveAll(this);
		MenuScreen->DestroyWindows();
	}

	MenuScreen = nullptr;
	MainMenuComponent->ReleaseResources();
	MainMenuComponent->SetWidget(nullptr);
}


void ACuboMenuActor::OnVisibilityChanged(ESlateVisibility Visibility)
{
	// Only check if the visibility actually changes
	if(LastScreenVisibility != Visibility)
	{
		LastScreenVisibility = Visibility;
		
		if(Visibility == ESlateVisibility::Hidden)
		{
			CloseScreen();
		}
	}
}

void ACuboMenuActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void ACuboMenuActor::ShowScreen(FString ID)
{
	if(MenuScreen)
	{
		SetActorHiddenInGame(false);
		MainMenuComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MainMenuComponent->SetCollisionResponseToChannel(ECollisionChannel::COLLISION_MainMenu, ECollisionResponse::ECR_Block);
		MenuScreen->ShowWindow(ID);
		SetActorTickEnabled(true);
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


void ACuboMenuActor::CloseScreen(bool bForceClose)
{
	if(MenuScreen && MenuScreen->CloseWindow(bForceClose))
	{
		SetActorHiddenInGame(true);
		MainMenuComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MainMenuComponent->SetCollisionResponseToChannel(ECollisionChannel::COLLISION_MainMenu, ECollisionResponse::ECR_Ignore);
	}
}