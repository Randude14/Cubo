// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CanvasPanelSlot.h"
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

	UPROPERTY(EditAnywhere, Category="Cubo")
	UTexture* CursorTexture;

	UPROPERTY(EditAnywhere, Category="Cubo")
	FVector2D CursorSize;

	UPROPERTY(EditAnywhere, Category="Cubo")
	FLinearColor DefaultLaserColor;

	UPROPERTY(EditAnywhere, Category="Cubo")
	FLinearColor SelectedPieceLaserColor;

	// How far the laser pointer should trace from the controller in world units
	UPROPERTY(EditAnywhere, Category="Cubo")
	float MaxLaserDistance;

	UPROPERTY()
	ACuboHandController* OtherController;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdateLaserBeam();

	UPROPERTY()
	UMaterialInstanceDynamic* LaserMaterialInstance;

	UPROPERTY()
	UWidgetComponent* HoveredWidgetComponent;

	UPROPERTY()
	class UImage* CursorImage;

protected:
	float MovingTimer;
	bool bGrabBeingPressed = false;
	bool bPieceDragging = false;

	UPROPERTY()
	AActor* SelectedActor;

	UPROPERTY()
	ACuboPiece* CuboPiece;

	UPROPERTY()
	ACuboGrid* Grid;

	UPROPERTY()
	UCanvasPanelSlot* CursorSlot;

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
