// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboPawn.h"


// Sets default values
ACuboPawn::ACuboPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnRootComponent = CreateDefaultSubobject<USceneComponent>("PawnRootComponent");
	SetRootComponent(PawnRootComponent);
	
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
}

// Grab functions
void ACuboPawn::LeftGrabPressed()
{
	
}

void ACuboPawn::RightGrabPressed()
{
	
}

void ACuboPawn::LeftGrabRelased()
{
	
}

void ACuboPawn::RightGrabReleased()
{
	
}

// Accelerate functions
void ACuboPawn::LeftAcceleratePressed()
{
	
}

void ACuboPawn::RightAcceleratePressed()
{
	
}

void ACuboPawn::LeftAccelerateReleased()
{
	
}

void ACuboPawn::RightAccelerateReleased()
{
	
}

// Rotate functions
void ACuboPawn::LeftRotatePiece()
{
	UE_LOG(LogTemp, Warning, TEXT("Move Piece Left"))
}

void ACuboPawn::RightRotatePiece()
{
	
}

// Move piece functions
void ACuboPawn::MovePieceLeftController(float Axis)
{
	if(Axis > 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Move Piece Left"))
	}
	else if(Axis < 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Move Piece Left"))
	}
}

void ACuboPawn::MovePieceRightController(float Axis)
{
	if(Axis > 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Move Piece Right"))
	}
	else if(Axis < 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Move Piece Right"))
	}
}
