// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboHandController.h"
#include "Components/WidgetInteractionComponent.h"
#include "Cubo/CuboFramework/CuboPiece.h"
#include "Kismet/GameplayStatics.h"

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

	TArray<AActor*> Grids;
	UGameplayStatics::GetAllActorsOfClass(this, ACuboGrid::StaticClass(), Grids);

	if(Grids.Num() > 0)
	{
		Grid = Cast<ACuboGrid>(Grids[0]);
	}
}

// Called every frame
void ACuboHandController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateLaserBeam();
	
	if(MovingTimer > 0)
	{
		MovingTimer -= DeltaTime;
	}
}

void ACuboHandController::UpdateLaserBeam()
{
	UWorld* World = GetWorld();

	if(World == nullptr || bGrabBeingPressed)
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

	// Don't interfere with other controllers actors
	if(OtherController->SelectedActor && OtherController->SelectedActor->GetAttachParentActor() == this->SelectedActor)
	{
		SelectedActor = nullptr;
		return;
	}

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

	if(SelectedActor != CuboPiece)
	{
		if(CuboPiece)
		{
			CuboPiece->Unhighlight();
		}

		if(SelectedActor)
		{
			CuboPiece = Cast<ACuboPiece>(SelectedActor->GetAttachParentActor());

			if(CuboPiece)
			{
				CuboPiece->Highlight();
			}
		}
		else
		{
			CuboPiece = nullptr;
		}
	}
}

void ACuboHandController::GrabPressed()
{
	bGrabBeingPressed = true;

	if(LaserBeam->IsVisible())
	{
		LaserBeam->SetVisibility(false);
	}
}

void ACuboHandController::GrabReleased()
{
	bGrabBeingPressed = false;

	if(! LaserBeam->IsVisible())
	{
		LaserBeam->SetVisibility(true);
	}
}

void ACuboHandController::AcceleratePressed()
{
	if(Grid)
	{
		Grid->SetAccelerate(true);
	}
}

void ACuboHandController::AccelerateReleased()
{
	if(Grid)
	{
		Grid->SetAccelerate(false);
	}
}

void ACuboHandController::TryRotatePiece()
{
	if(CuboPiece)
	{
		Grid->TryRotatePiece();
	}
}

void ACuboHandController::MovePieceStopped()
{
	MovingTimer = 0.f;
}

void ACuboHandController::TryMovePieceLeft()
{
	if(CuboPiece && bGrabBeingPressed && MovingTimer <= 0.f)
	{
		Grid->TryMovePiece(false);
		MovingTimer = ControllerMoveInfo.MovePieceTime;            
	}
}

void ACuboHandController::TryMovePieceRight()
{
	if(CuboPiece && bGrabBeingPressed && MovingTimer <= 0.f)
	{
		Grid->TryMovePiece(true);
		MovingTimer = ControllerMoveInfo.MovePieceTime;
	}
}