// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboPawn.h"

#include "HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Cubo/CuboFramework/CuboPlayerController.h"
#include "Cubo/UI/CuboMenuActor.h"

// Sets default values
ACuboPawn::ACuboPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnRootComponent = CreateDefaultSubobject<USceneComponent>("PawnRootComponent");
	SetRootComponent(PawnRootComponent);

	KbmCameraLocation = CreateDefaultSubobject<USceneComponent>("KbmCameraLocation");
	KbmCameraLocation->SetupAttachment(PawnRootComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("VRCameraComponent");
	CameraComponent->SetupAttachment(PawnRootComponent);
	
	RightControllerComponent = CreateDefaultSubobject<UMotionControllerComponent>("RightController");
	RightControllerComponent->SetupAttachment(PawnRootComponent);
	LeftControllerComponent = CreateDefaultSubobject<UMotionControllerComponent>("LeftController");
	LeftControllerComponent->SetupAttachment(PawnRootComponent);
}

// Called when the game starts or when spawned
void ACuboPawn::BeginPlay()
{
	Super::BeginPlay();

	if(RightHandControllerClass && LeftHandControllerClass)
	{
		RightController = GetWorld()->SpawnActor<ACuboHandController>(RightHandControllerClass);
		RightController->AttachToComponent(RightControllerComponent, FAttachmentTransformRules::KeepRelativeTransform, FName("RightController"));
		LeftController = GetWorld()->SpawnActor<ACuboHandController>(LeftHandControllerClass);
		LeftController->AttachToComponent(LeftControllerComponent, FAttachmentTransformRules::KeepRelativeTransform, FName("LeftController"));
		
		RightController->OtherController = LeftController;
		LeftController->OtherController = RightController;

		RightController->ControlsDisplay->SetVisibility(false, true);
		LeftController->ControlsDisplay->SetVisibility(false, true);
	}

	if(! UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayConnected())
	{
		CameraComponent->SetRelativeLocation( KbmCameraLocation->GetRelativeLocation() );
	}

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, ACuboMenuActor::StaticClass(), Actors);

	if(Actors.Num() > 0)
	{
		MenuActor = Cast<ACuboMenuActor>(Actors[0]);
	}
}

void ACuboPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACuboPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("RightGrab", EInputEvent::IE_Pressed, this, &ACuboPawn::RightGrabPressed);
	PlayerInputComponent->BindAction("LeftGrab", EInputEvent::IE_Pressed, this, &ACuboPawn::LeftGrabPressed);
	PlayerInputComponent->BindAction("RightGrab", EInputEvent::IE_Released, this, &ACuboPawn::RightGrabReleased);
	PlayerInputComponent->BindAction("LeftGrab", EInputEvent::IE_Released, this, &ACuboPawn::LeftGrabRelased);

	PlayerInputComponent->BindAction("RightAccelerate", EInputEvent::IE_Pressed, this, &ACuboPawn::RightAcceleratePressed);
	PlayerInputComponent->BindAction("LeftAccelerate", EInputEvent::IE_Pressed, this, &ACuboPawn::LeftAcceleratePressed);
	PlayerInputComponent->BindAction("RightAccelerate", EInputEvent::IE_Released, this, &ACuboPawn::RightAccelerateReleased);
	PlayerInputComponent->BindAction("LeftAccelerate", EInputEvent::IE_Released, this, &ACuboPawn::LeftAccelerateReleased);

	PlayerInputComponent->BindAction("RightRotatePiece", EInputEvent::IE_Pressed, this, &ACuboPawn::RightRotatePiece);
	PlayerInputComponent->BindAction("LeftRotatePiece", EInputEvent::IE_Pressed, this, &ACuboPawn::LeftRotatePiece);

	PlayerInputComponent->BindAxis("MovePieceLeftController", this, &ACuboPawn::MovePieceLeftController);
	PlayerInputComponent->BindAxis("MovePieceRightController", this, &ACuboPawn::MovePieceRightController);

	PlayerInputComponent->BindAction("MenuAction", EInputEvent::IE_Pressed, this, &ACuboPawn::MenuButtonPressed);
	PlayerInputComponent->BindAction("ControllerTipSummon", EInputEvent::IE_Pressed, this, &ACuboPawn::ControllerTipSummon);
}

void ACuboPawn::MenuButtonPressed()
{
	if(MenuActor)
	{
		if(ACuboPlayerController* PlayerController = Cast<ACuboPlayerController>( GetController() ))
		{
			if(ACuboGrid* Grid = PlayerController->GetOwningGrid())
			{
				if(MenuActor->IsHidden())
				{
					if(! Grid->IsPaused())
					{
						Grid->Pause();
						MenuActor->ShowScreen("PauseMenu");
					}
				}
				else if(MenuActor->CanForceClose())
				{
					Grid->Resume();
					MenuActor->CloseScreen();
				}
			}
		}
	}
}

void ACuboPawn::ControllerTipSummon()
{
	if(LeftController)
	{
		LeftController->ControlsDisplay->SetVisibility( ! LeftController->ControlsDisplay->IsVisible(), true );
	}
	if(RightController)
	{
		RightController->ControlsDisplay->SetVisibility( ! RightController->ControlsDisplay->IsVisible(), true );
	}
}

// Grab functions
void ACuboPawn::LeftGrabPressed()
{
	if(LeftController)
	{
		LeftController->GrabPressed();
	}
}

void ACuboPawn::RightGrabPressed()
{
	if(RightController)
	{
		RightController->GrabPressed();
	}
}

void ACuboPawn::LeftGrabRelased()
{
	if(LeftController)
	{
		LeftController->GrabReleased();
	}
}

void ACuboPawn::RightGrabReleased()
{
	if(RightController)
	{
		RightController->GrabReleased();
	}
}

// Accelerate functions
void ACuboPawn::LeftAcceleratePressed()
{
	if(LeftController)
	{
		LeftController->AcceleratePressed();
		LeftController->LaserDirection->PressKey(EKeys::LeftMouseButton);
	}
}

void ACuboPawn::RightAcceleratePressed()
{
	if(RightController)
	{
		RightController->AcceleratePressed();
	}
}

void ACuboPawn::LeftAccelerateReleased()
{
	if(LeftController)
	{
		LeftController->AccelerateReleased();
		LeftController->LaserDirection->ReleaseKey(EKeys::LeftMouseButton);
	}
}

void ACuboPawn::RightAccelerateReleased()
{
	if(RightController)
	{
		RightController->AccelerateReleased();
	}
}

// Rotate functions
void ACuboPawn::LeftRotatePiece()
{
	if(LeftController)
	{
		LeftController->TryRotatePiece();
	}
}

void ACuboPawn::RightRotatePiece()
{
	if(RightController)
	{
		RightController->TryRotatePiece();
	}
}

// Move piece functions
void ACuboPawn::MovePieceLeftController(float Axis)
{
	if(Axis > 0.f)
	{
		if(LeftController)
		{
			LeftController->TryMovePieceRight();
		}
	}
	else if(Axis < 0.f)
	{
		if(LeftController)
		{
			LeftController->TryMovePieceLeft();
		}
	}
	else if(LeftController)
	{
		LeftController->MovePieceStopped();
	}
}

void ACuboPawn::MovePieceRightController(float Axis)
{
	if(Axis > 0.f)
	{
		if(RightController)
		{
			RightController->TryMovePieceRight();
		}
	}
	else if(Axis < 0.f)
	{
		if(RightController)
		{
			RightController->TryMovePieceLeft();
		}
	}
	else if(RightController)
	{
		RightController->MovePieceStopped();
	}
}
