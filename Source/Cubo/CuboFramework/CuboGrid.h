// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CuboPieceQueue.h"
#include "Cubo/CuboBlock.h"
#include "GameFramework/Actor.h"
#include "CuboGrid.generated.h"

class ACuboPiece;

struct FCuboGridLocation
{
	int X, Y;
};

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

	// When enabled, will show lines out the occupied spaces in the grid, for editor debugging only
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	bool bShowGridSpaces=false;

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

	bool IsBlockHere(FCuboGridLocation Pos);
	bool IsBlockHere(int Row, int Col);

	void SetBlock(FCuboGridLocation Pos, ACuboBlock* Block);
	void SetBlock(int Row, int Col, ACuboBlock* Block);

	bool TryMovePieceDown();

	bool IsPieceInLegalSpot();

	FCuboGridLocation ConvertToGridSpace(FVector Location);

public:
	void SetAccelerate(bool bAccelerate);
	void TryRotatePiece();
	void TryMovePiece(bool bRight);
};
