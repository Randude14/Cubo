// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboHandController.h"

#include "DrawDebugHelpers.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/WidgetInteractionComponent.h"
#include "Cubo/CuboFramework/CuboPiece.h"
#include "Cubo/UI/CuboMenuActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACuboHandController::ACuboHandController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ControllerRoot = CreateDefaultSubobject<USceneComponent>("ControllerRoot");
	SetRootComponent(ControllerRoot);

	ControlsDisplay = CreateDefaultSubobject<USceneComponent>("ControlsDisplay");
	ControlsDisplay->SetupAttachment(ControllerRoot);

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

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, ACuboGrid::StaticClass(), Actors);

	if(Actors.Num() > 0)
	{
		Grid = Cast<ACuboGrid>(Actors[0]);
	}

	UseKbm = ! UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayConnected();
}

bool ACuboHandController::IsControllerGrabbing()
{
	return bGrabBeingPressed || UseKbm;
}

// Called every frame
void ACuboHandController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateLaserBeam();
	
	if(bUseMotion && MovingTimer > 0)
	{
		MovingTimer -= DeltaTime;
	}

	if(bUseMotion && CuboPiece && CuboPiece->IsBeingGrabbed() && Grid)
	{
		FCuboGridLocation CurrentDragged = Grid->LinePlaneIntersect(CuboPiece, LaserBeam->GetComponentLocation(), LaserBeam->GetForwardVector());

		if( FMath::Abs(DraggedLocation.X - CurrentDragged.X) >= 2 )
		{
			int Delta = DraggedLocation.X - CurrentDragged.X;

			if(Grid->TryMovePieceRL(CuboPiece, (Delta < 0) ? true : false ))
			{
				DraggedLocation = CurrentDragged;
			}
		}
	}
}

void ACuboHandController::UpdateLaserBeam()
{
	UWorld* World = GetWorld();

	if(World == nullptr || (!bDebugHand && IsControllerGrabbing()) )
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

	ACuboPiece* SelectedPiece = nullptr;
	
	if(SelectedActor && SelectedActor->GetAttachParentActor())
	{
		SelectedPiece = Cast<ACuboPiece>(SelectedActor->GetAttachParentActor());
	}

	if(! SelectedPiece && SelectedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ditto      %s"), *SelectedActor->GetName())
	}
	
	if(SelectedPiece != CuboPiece)
	{
		if(CuboPiece && (! OtherController || OtherController->CuboPiece != CuboPiece) )
		{
			CuboPiece->Unhighlight();
		}

		if(SelectedActor)
		{
			CuboPiece = SelectedPiece;

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

	if(CuboPiece && ! CuboPiece->IsBeingGrabbed())
	{
		CuboPiece->GrabPiece(this);
		
		if(Grid && !Grid->IsPaused())
		{
			DraggedLocation = Grid->LinePlaneIntersect(CuboPiece, LaserBeam->GetComponentLocation(), LaserBeam->GetForwardVector());
		}
	}
}

void ACuboHandController::GrabReleased()
{
	bGrabBeingPressed = false;

	if(! LaserBeam->IsVisible())
	{
		LaserBeam->SetVisibility(true);
	}

	if(CuboPiece)
	{
		CuboPiece->ReleasePiece(this);
	}
}

void ACuboHandController::AcceleratePressed()
{
	if(Grid && !Grid->IsPaused())
	{
		if(CuboPiece)
		{
			CuboPiece->SetAccelerate(true);
		}
		else
		{
			LaserDirection->PressPointerKey(EKeys::LeftMouseButton);
		}
	}
	else
	{
		LaserDirection->PressPointerKey(EKeys::LeftMouseButton);
	}
}

void ACuboHandController::AccelerateReleased()
{
	if(Grid && !Grid->IsPaused())
	{
		if(CuboPiece)
		{
			CuboPiece->SetAccelerate(false);
		}
		else
		{
			LaserDirection->ReleasePointerKey(EKeys::LeftMouseButton);
		}
	}
	else
	{
		LaserDirection->ReleasePointerKey(EKeys::LeftMouseButton);
	}
}

void ACuboHandController::TryRotatePiece()
{
	if(Grid && !Grid->IsPaused())
	{
		Grid->TryRotatePiece(CuboPiece);
	}
}

void ACuboHandController::MovePieceStopped()
{
	MovingTimer = 0.f;
}

void ACuboHandController::TryMovePieceLeft()
{
	if(!bUseMotion && Grid && !Grid->IsPaused() && CuboPiece && IsControllerGrabbing() && MovingTimer <= 0.f)
	{
		Grid->TryMovePieceRL(CuboPiece, false);
		MovingTimer = ControllerMoveInfo.MovePieceTime;            
	}
}

void ACuboHandController::TryMovePieceRight()
{
	if(!bUseMotion && Grid && !Grid->IsPaused() && CuboPiece && IsControllerGrabbing() && MovingTimer <= 0.f)
	{
		Grid->TryMovePieceRL(CuboPiece, true);
		MovingTimer = ControllerMoveInfo.MovePieceTime;
	}
}