// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboPlayerController.h"

#include "Kismet/GameplayStatics.h"

void ACuboPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	OwningGrid = GetActorOfClassFromWorld<ACuboGrid>(ACuboGrid::StaticClass());
	MenuActor = GetActorOfClassFromWorld<ACuboMenuActor>(ACuboMenuActor::StaticClass());
}

template <class T>
T* ACuboPlayerController::GetActorOfClassFromWorld(UClass* Class)
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, Class, Actors);

	if(Actors.Num())
	{
		T* ResultActor = Cast<T>(Actors[0]);
		return ResultActor;
	}
	return nullptr;
}

ACuboGrid* ACuboPlayerController::GetOwningGrid() const
{
	return OwningGrid;
}

ACuboMenuActor* ACuboPlayerController::GetMenuActor() const
{
	return MenuActor;
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