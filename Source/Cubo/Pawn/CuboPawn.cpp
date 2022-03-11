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
	
	RightController = CreateDefaultSubobject<UMotionControllerComponent>("RightController");
	RightController->SetupAttachment(PawnRootComponent);
	LeftController = CreateDefaultSubobject<UMotionControllerComponent>("LeftController");
	LeftController->SetupAttachment(PawnRootComponent);
}

// Called when the game starts or when spawned
void ACuboPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACuboPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACuboPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

