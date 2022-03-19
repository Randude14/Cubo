// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CuboPieceQueue.h"
#include "Cubo/CuboBlock.h"
#include "GameFramework/Actor.h"
#include "CuboGrid.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCuboGridScoreChanged, const class ACuboGrid*, Grid, const int32&, ScoreAdded);

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

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACuboBlock> HighlightBlockClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACuboBlock> RotateErrorBlockClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	int32 GridWidth=10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	int32 GridHeight=20;

	// The color the cube blocks will have if the player has gone out of bounds at the top
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	FLinearColor OutOfBoundsColor;

	// The amount of time the rotate error piece should be visible
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	float RotateErrorVisibleTime=0.5f;

	// When enabled, will show lines out the occupied spaces in the grid, for editor debugging only
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	bool bShowGridSpaces=false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	TArray<int32> ScoreByLines;

	UPROPERTY(BlueprintAssignable, Category="Cubo|Grid")
	FCuboGridScoreChanged ScoreChanged;

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

	UPROPERTY()
	ACuboPiece* HighlighterPiece;

	UPROPERTY()
	ACuboPiece* RotateErrorPiece;

	bool bShouldAccelerate = false;
	bool bGamePaused = false;
	float PieceMoveTimer = 0.f;
	float RotateErrorTimer;
	
	int32 GridScore = 0.f;

	// used when determine legal spots for the cubo pieces
	// helps account for rounding errors when dealing with floats in grid spaces
	float TinyPieceOffset;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsBlockHere(FCuboGridLocation Pos);
	bool IsBlockHere(int Row, int Col);

	void SetBlock(FCuboGridLocation Pos, ACuboBlock* Block);
	void SetBlock(int Row, int Col, ACuboBlock* Block);
	
	bool TryMovePieceDown(ACuboPiece* Piece, bool bSpawnBlocks=true);

	bool IsPieceInLegalSpot(ACuboPiece* Piece);

	FCuboGridLocation ConvertToGridSpace(FVector Location);

	void UpdateHighlighter();
	
	void CheckFilledLines();

public:
	void SetAccelerate(bool bAccelerate);
	void TryRotatePiece();
	void TryMovePieceRL(bool bRight);

	UFUNCTION(BlueprintCallable, Category="Cubo")
	void Pause()
	{
		bGamePaused = true;
	};

	UFUNCTION(BlueprintCallable, Category="Cubo")
	void Resume()
	{
		bGamePaused = false;
	};

	UFUNCTION(BlueprintCallable, Category="Cubo")
	bool IsPaused() const
	{
		return bGamePaused;
	}

	UFUNCTION(BlueprintCallable, Category="Cubo")
	int32 GetGridScore() const
	{
		return GridScore;
	};
};
