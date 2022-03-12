// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboHandController.h"
#include "Components/WidgetInteractionComponent.h"


// Sets default values
ACuboHandController::ACuboHandController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ControllerRoot = CreateDefaultSubobject<USceneComponent>("ControllerRoot");
	SetRootComponent(ControllerRoot);

	LaserDirection = CreateDefaultSubobject<UWidgetInteractionComponent>("LaserDirection");
	LaserDirection->SetMobility(EComponentMobility::Movable);
	LaserDirection->SetupAttachment(ControllerRoot);

	LaserSpline = CreateDefaultSubobject<USplineComponent>("LaserSpline");
	LaserSpline->SetMobility(EComponentMobility::Movable);
	LaserSpline->SetupAttachment(LaserDirection);
	
	LaserBeam = CreateDefaultSubobject<USplineMeshComponent>("LaserBeamMesh");
	LaserBeam->SetMobility(EComponentMobility::Movable);
	LaserBeam->SetupAttachment(LaserDirection);

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

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(OtherController);
	
	FHitResult ObjectHitResult;
	FVector LaserLocation = LaserDirection->GetComponentLocation();
	FVector LaserForward = LaserDirection->GetForwardVector() * MaxLaserDistance + LaserLocation;
	bool bHitObject = World->LineTraceSingleByChannel(ObjectHitResult, LaserLocation, LaserForward, ECollisionChannel::ECC_Visibility);
	
	SelectedActor = ObjectHitResult.GetActor();

	FVector HitLocation = bHitObject ? ObjectHitResult.Location : LaserForward;
	LaserSpline->SetLocationAtSplinePoint(0, LaserLocation, ESplineCoordinateSpace::World);
	LaserSpline->SetTangentAtSplinePoint(0, FVector::ZeroVector, ESplineCoordinateSpace::World);
	LaserSpline->SetLocationAtSplinePoint(1, HitLocation, ESplineCoordinateSpace::World);
	LaserSpline->SetTangentAtSplinePoint(1, FVector::ZeroVector, ESplineCoordinateSpace::World);

	FVector LaserPointStart = LaserSpline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::Local);
	FVector LaserPointTanStart = LaserSpline->GetTangentAtSplinePoint(0, ESplineCoordinateSpace::Local);
	FVector LaserPointEnd = LaserSpline->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	FVector LaserPointTanEnd = LaserSpline->GetTangentAtSplinePoint(1, ESplineCoordinateSpace::Local);
	
	LaserBeam->SetStartAndEnd(LaserPointStart, LaserPointTanStart, LaserPointEnd, LaserPointTanEnd, true);

	if(SelectedActor != CuboBlock)
	{
		if(CuboBlock)
		{
			CuboBlock->Unhighlight();
		}

		CuboBlock = Cast<ACuboBlock>(SelectedActor);

		if(CuboBlock)
		{
			CuboBlock->Highlight();
		}
	}
}

void ACuboHandController::GrabPressed()
{
	
}

void ACuboHandController::GrabReleased()
{
	
}

void ACuboHandController::AcceleratePressed()
{
	
}

void ACuboHandController::AccelerateReleased()
{
	
}

void ACuboHandController::TryRotatePiece()
{
	
}

void ACuboHandController::TryMovePieceLeft()
{
	
}

void ACuboHandController::TryMovePieceRight()
{
	
}