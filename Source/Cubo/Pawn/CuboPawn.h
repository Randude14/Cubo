// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CuboHandController.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "CuboPawn.generated.h"

class UMotionControllerComponent;
class ACuboHandController;

UCLASS()
class CUBO_API ACuboPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACuboPawn();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* PawnRootComponent;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere)
	UMotionControllerComponent* LeftControllerComponent;
	
	UPROPERTY(VisibleAnywhere)
	UMotionControllerComponent* RightControllerComponent;

	UPROPERTY()
	ACuboHandController* LeftController;

	UPROPERTY()
	ACuboHandController* RightController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	TSubclassOf<ACuboHandController> LeftHandControllerClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	TSubclassOf<ACuboHandController> RightHandControllerClass;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
