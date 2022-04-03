// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboHandController.h"

#include "DrawDebugHelpers.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/WidgetInteractionComponent.h"
#include "Components/Image.h"
#include "Cubo/CuboFramework/CuboPiece.h"
#include "Cubo/CuboFramework/CuboPlayerController.h"
#include "Cubo/UI/CuboMenuActor.h"
#include "Kismet/GameplayStatics.h"


// Define custom channels
#define COLLISION_MainMenu		ECC_GameTraceChannel1


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

	if(UMaterialInterface* LaserMat = LaserBeam->GetMaterial(0))
	{
		if(UMaterialInstanceDynamic* Instance = UMaterialInstanceDynamic::Create(LaserMat, LaserBeam))
		{
			LaserBeam->SetMaterial(0, Instance);
			LaserBeam->SetMaterial(1, Instance);
			Instance->SetVectorParameterValue(FName("LaserColor"), DefaultLaserColor);
			LaserMaterialInstance = Instance;
		}
	}

	if(CursorTexture)
	{
		CursorImage = NewObject<UImage>(this, FName("CursorImage"));
		CursorImage->Brush.SetResourceObject(CursorTexture);
		CursorImage->SetVisibility(ESlateVisibility::HitTestInvisible);
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

	if(ACuboPlayerController* PlayerController = Cast<ACuboPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		if(PlayerController->UseMotionControls() && CuboPiece && CuboPiece->IsBeingGrabbed() && Grid && bPieceDragging && (!CuboPiece->IsBoostOn() || !PlayerController->ShouldLockPieceOnBoost()) )
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
}

void ACuboHandController::UpdateLaserBeam()
{
	UWorld* World = GetWorld();

	if(World == nullptr || (bGrabBeingPressed && CuboPiece))
	{
		return;
	}

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(OtherController);
	
	FHitResult ObjectHitResult, WidgetHitResult;
	FVector LaserLocation = LaserDirection->GetComponentLocation();
	FVector LaserForward = LaserDirection->GetForwardVector() * MaxLaserDistance + LaserLocation;
	bool bHitObject = World->LineTraceSingleByChannel(ObjectHitResult, LaserLocation, LaserForward, ECollisionChannel::ECC_Visibility);
	bool bHitWidget = World->LineTraceSingleByChannel(WidgetHitResult, LaserLocation, LaserForward, ECollisionChannel::COLLISION_MainMenu);

	FVector HitLocation;
	
	if(! bHitWidget)
	{
		SelectedActor = ObjectHitResult.GetActor();
		HitLocation = bHitObject ? ObjectHitResult.Location : LaserForward;
	}
	else
	{
		SelectedActor = nullptr;
		HitLocation = bHitWidget ? WidgetHitResult.Location : LaserForward;
	}

	if(CursorImage)
	{
		if(UWidgetComponent* Component = Cast<UWidgetComponent>( WidgetHitResult.GetComponent() ))
		{
			if(CursorSlot == nullptr)
			{
				HoveredWidgetComponent = Component;

				if(UUserWidget* UserWidget = Component->GetUserWidgetObject())
				{
					if(UCanvasPanel* Panel = Cast<UCanvasPanel>(UserWidget->GetRootWidget()))
					{
						CursorSlot = Panel->AddChildToCanvas(CursorImage);
						CursorSlot->SetZOrder(5);
					}
				}
			}
			if(CursorSlot)
			{
				CursorSlot->SetSize(CursorSize);
				CursorSlot->SetAlignment(FVector2D(0.5, 0.5));

				FVector CursorHitLocation = HitLocation - Component->GetComponentLocation();
				FVector2D CursorLocation( CursorHitLocation.Y, CursorHitLocation.Z );
				CursorLocation.X = FMath::Abs(CursorLocation.X);
				CursorLocation.Y = FMath::Abs(CursorLocation.Y);
				CursorSlot->SetPosition(CursorLocation);
			}
		}
		else if(HoveredWidgetComponent)
		{
			CursorImage->RemoveFromParent();
			CursorSlot = nullptr;
			HoveredWidgetComponent = nullptr;
		}
	}

	
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
	
	if(SelectedPiece != CuboPiece)
	{
		if(CuboPiece && (! OtherController || OtherController->CuboPiece != CuboPiece) )
		{
			CuboPiece->Unhighlight();
			if(LaserMaterialInstance)
			{
				LaserMaterialInstance->SetVectorParameterValue(FName("LaserColor"), DefaultLaserColor);
			}
		}

		if(SelectedActor)
		{
			CuboPiece = SelectedPiece;

			if(CuboPiece)
			{
				CuboPiece->Highlight();
				if(LaserMaterialInstance)
				{
					LaserMaterialInstance->SetVectorParameterValue(FName("LaserColor"), SelectedPieceLaserColor);
				}
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

	if(ACuboPlayerController* PlayerController = Cast<ACuboPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		if(CuboPiece && ! CuboPiece->IsBeingGrabbed())
		{
			CuboPiece->GrabPiece(this);
			
			if(LaserBeam->IsVisible())
			{
				LaserBeam->SetVisibility(false);
			}
		
			if(Grid && !Grid->IsPaused())
			{
				bPieceDragging = true;
				DraggedLocation = Grid->LinePlaneIntersect(CuboPiece, LaserBeam->GetComponentLocation(), LaserBeam->GetForwardVector());
			}
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
		bPieceDragging = false;
		CuboPiece->ReleasePiece(this);
	}
}

void ACuboHandController::AcceleratePressed()
{
	if(Grid && !Grid->IsPaused())
	{
		if(CuboPiece)
		{
			CuboPiece->SetBoosting(true);
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
			CuboPiece->SetBoosting(false);
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
	if(ACuboPlayerController* PlayerController = Cast<ACuboPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		if(!PlayerController->UseMotionControls() && Grid && !Grid->IsPaused() && CuboPiece && (!CuboPiece->IsBoostOn() || !PlayerController->ShouldLockPieceOnBoost()) && bGrabBeingPressed && MovingTimer <= 0.f)
		{
			Grid->TryMovePieceRL(CuboPiece, false);
			MovingTimer = ControllerMoveInfo.MovePieceTime;            
		}
	}
}

void ACuboHandController::TryMovePieceRight()
{
	if(ACuboPlayerController* PlayerController = Cast<ACuboPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		if(!PlayerController->UseMotionControls() && Grid && !Grid->IsPaused() && CuboPiece && (!CuboPiece->IsBoostOn() || !PlayerController->ShouldLockPieceOnBoost()) && bGrabBeingPressed && MovingTimer <= 0.f)
		{
			Grid->TryMovePieceRL(CuboPiece, true);
			MovingTimer = ControllerMoveInfo.MovePieceTime;
		}
	}
}