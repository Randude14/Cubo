// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboGrid.h"


// Sets default values
ACuboGrid::ACuboGrid()
{
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
	SetRootComponent(RootSceneComponent);
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACuboGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACuboGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

