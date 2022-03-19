// Fill out your copyright notice in the Description page of Project Settings.


#include "GridDisplay.h"


AGridDisplay::AGridDisplay()
{
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("RootScene");
	SetRootComponent(RootSceneComponent);

	DisplayComponent = CreateDefaultSubobject<UWidgetComponent>("DisplayUI");
	DisplayComponent->SetupAttachment(RootSceneComponent);
}

void AGridDisplay::BeginPlay()
{
	Super::BeginPlay();
}

void AGridDisplay::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

