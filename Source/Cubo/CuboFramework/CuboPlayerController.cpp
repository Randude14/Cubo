// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboPlayerController.h"

#include "Kismet/GameplayStatics.h"

void ACuboPlayerController::BeginPlay()
{
	TArray<AActor*> Grids;
	UGameplayStatics::GetAllActorsOfClass(this, ACuboGrid::StaticClass(), Grids);

	if(Grids.Num())
	{
		OwningGrid = Cast<ACuboGrid>(Grids[0]);
	}
}

ACuboGrid* ACuboPlayerController::GetOwningGrid() const
{
	return OwningGrid;
}

void ACuboPlayerController::SaveSettings()
{
	
}

bool ACuboPlayerController::LockOnAccelerate()
{
	return true;
}

bool ACuboPlayerController::IsMotionControls()
{
	return true;
}