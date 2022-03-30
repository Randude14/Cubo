// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CuboGrid.h"
#include "UObject/Object.h"
#include "CuboPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CUBO_API ACuboPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	void SaveSettings();

	bool IsMotionControls();

	bool LockOnAccelerate();

	ACuboGrid* GetOwningGrid() const;

protected:
	ACuboGrid* OwningGrid;
};
