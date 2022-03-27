
#include "CuboMenuActor.h"


ACuboMenuActor::ACuboMenuActor()
{
	MenuRoot = CreateDefaultSubobject<USceneComponent>("MenuRoot");
	SetRootComponent(MenuRoot);
	
	MainMenuComponent = CreateDefaultSubobject<UWidgetComponent>("MainMenu");
	MainMenuComponent->SetupAttachment(MenuRoot);
	MainMenuComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ACuboMenuActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACuboMenuActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

