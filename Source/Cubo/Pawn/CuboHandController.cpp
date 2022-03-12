// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboHandController.h"


// Sets default values
ACuboHandController::ACuboHandController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ControllerRoot = CreateDefaultSubobject<USceneComponent>("ControllerRoot");
	SetRootComponent(ControllerRoot);

	LaserBeam = CreateDefaultSubobject<UStaticMeshComponent>("LaserBeamMesh");
	LaserBeam->SetupAttachment(ControllerRoot);

	MaxLaserDistance = 300.f;
}

// Called when the game starts or when spawned
void ACuboHandController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACuboHandController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateLaserBeam();
}

void ACuboHandController::UpdateLaserBeam()
{
	UWorld* World = GetWorld();

	if(World == nullptr)
	{
		return;
	}

	FVector LaserLocation = LaserBeam->GetComponentLocation();
	FVector LaserForward = LaserBeam->GetForwardVector() * MaxLaserDistance; // TODO: maybe turn into a uproperty?
	bool bHitObject = World->LineTraceSingleByChannel(ObjectHitResult, LaserLocation, LaserForward, ECollisionChannel::ECC_Visibility);

	float BeamDistance = MaxLaserDistance;

	if(bHitObject)
	{
		BeamDistance = ObjectHitResult.Distance;
	}
	
	FVector Scale = LaserBeam->GetComponentScale();
	Scale.X = BeamDistance;

	LaserBeam->SetWorldScale3D(Scale);
}


