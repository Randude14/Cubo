// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CuboPieceQueue.h"
#include "Cubo/CuboBlock.h"
#include "GameFramework/Actor.h"
#include "CuboGrid.generated.h"

class ACuboPiece;

UCLASS()
class CUBO_API ACuboGrid : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACuboGrid();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootSceneComponent;

	UPROPERTY(VisibleAnywhere)
	UChildActorComponent* PieceQueueComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	int32 GridWidth=10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	int32 GridHeight=20;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	ACuboPieceQueue* PieceQueue;

	// Grid is represented as a map, 2d positions are referenced as an index by i = r*width + c
	UPROPERTY()
	TMap<int32, ACuboBlock*> Grid;

	UPROPERTY()
	ACuboPiece* CurrentPiece;

	bool bShouldAccelerate = false;
	float PieceMoveTimer = 0.f;

	// used when determine legal spots for the cubo pieces
	// helps account for rounding errors when dealing with floats in grid spaces
	float TinyPieceOffset;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsBlockHere(FVector2D Pos);
	bool IsBlockHere(int Row, int Col);

	void SetBlock(FVector2D Pos, ACuboBlock* Block);
	void SetBlock(int Row, int Col, ACuboBlock* Block);

	bool TryMovePieceDown();

	bool IsPieceInLegalSpot();

public:
	void SetAccelerate(bool bAccelerate);
	void TryRotatePiece();
	void TryMovePiece(bool bRight);
};
