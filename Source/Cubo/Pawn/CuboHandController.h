// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CuboHandController.generated.h"

UCLASS()
class CUBO_API ACuboHandController : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACuboHandController();

	UPROPERTY(EditAnywhere)
	USceneComponent* ControllerRoot;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LaserBeam;

	UPROPERTY(EditAnywhere, Category="Cubo")
	float MaxLaserDistance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void UpdateLaserBeam();
	FHitResult ObjectHitResult;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GrabPressed();
	void GrabReleased();

	void AcceleratePressed();
	void AccelerateReleased();

	void TryRotatePiece();
	void TryMovePieceLeft();
	void TryMovePieceRight();
};
