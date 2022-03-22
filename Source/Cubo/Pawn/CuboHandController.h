// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Cubo/CuboFramework/CuboGrid.h"
#include "GameFramework/Actor.h"
#include "CuboHandController.generated.h"

class ACuboPiece;


USTRUCT()
struct FControllerMoveInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Cubo")
	float MovePieceTime=0.5;
};

UCLASS()
class CUBO_API ACuboHandController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACuboHandController();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* ControllerRoot;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* ControlsDisplay;

	UPROPERTY(VisibleAnywhere)
	USplineComponent* LaserSpline;

	UPROPERTY(VisibleAnywhere)
	UWidgetInteractionComponent* LaserDirection;

	UPROPERTY(VisibleAnywhere)
	USplineMeshComponent* LaserBeam;

	UPROPERTY(EditAnywhere, Category="Cubo")
	FControllerMoveInfo ControllerMoveInfo;

	// How far the laser pointer should trace from the controller in world units
	UPROPERTY(EditAnywhere, Category="Cubo")
	float MaxLaserDistance;

	UPROPERTY(EditAnywhere, Category="Cubo")
	bool bUseMotion=true;

	UPROPERTY()
	ACuboHandController* OtherController;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdateLaserBeam();

	bool IsControllerGrabbing();


protected:
	float MovingTimer;
	bool bGrabBeingPressed = false;
	bool UseKbm = false;

	UPROPERTY()
	AActor* SelectedActor;

	UPROPERTY()
	ACuboPiece* CuboPiece;

	UPROPERTY()
	ACuboGrid* Grid;

	FCuboGridLocation DraggedLocation;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GrabPressed();
	void GrabReleased();

	void AcceleratePressed();
	void AccelerateReleased();

	void TryRotatePiece();

	void MovePieceStopped();
	void TryMovePieceLeft();
	void TryMovePieceRight();
};
