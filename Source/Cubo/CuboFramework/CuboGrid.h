// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cubo/CuboBlock.h"
#include "GameFramework/Actor.h"
#include "CuboGrid.generated.h"

class ACuboPiece;


USTRUCT(BlueprintType)
struct FPieceMoveInfo
{
	GENERATED_BODY()
	
	
	// The distance between the blocks in the grid
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	float BlockSpace=100.f;

	// The percentage the block will move between grid spaces
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	float MovePieceBy=0.25f;

	// The amount of time between moving intervals when the player is not speding it
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	float NormalTime=0.25f;

	// The amount of time between moving intervals when the player is accelerating it
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	float AccelerateTime=0.1f;

	float GetMoveDistance()
	{
		return BlockSpace * MovePieceBy;
	};
};

USTRUCT(BlueprintType)
struct FCuboPieceInfo
{
	GENERATED_BODY()
	
	/*
	* Pattern for this piece, Xs are for a cube, 0s for an empty space, and a space between each row
	* Each group of characters will represent a row, there must be at least one X in the pattern
	* or this piece will never spawn
	* */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	FString PiecePattern;

	// Color of this piece when it is spawned
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	FLinearColor CubeColor;
	
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

	// List of the cubo pieces that will be generated and spawned for the player to fit in the grid
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	TArray<FCuboPieceInfo> GeneratedCuboPieces;

	// The scale of spawned cubo blocks
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	FVector BlockScale;

	// Contains information about the block spacing and move distancews
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	FPieceMoveInfo PieceMoveInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	TSubclassOf<ACuboPiece> CuboPieceClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	TSubclassOf<ACuboBlock> CuboBlockClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	int32 GridWidth=10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	int32 GridHeight=20;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	float SpawnTime = 5.f;

	// The amount of pieces that are sitting idle
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	int32 IdlePieces = 5;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Grid is represented as a map, 2d positions are referenced as an index by i = r*width + c
	TMap<int32, ACuboPiece*> Grid;
	TArray<ACuboPiece*> Cubos;

	UPROPERTY()
	ACuboPiece* CurrentPiece;

	bool bShouldAccelerate = false;
	float PieceMoveTimer = 0.f;
	float Timer = 0.f;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsPieceHere(FVector2D Pos);
	bool IsPieceHere(int Row, int Col);

	bool SetPiece(int Row, int Col, ACuboPiece* Piece);

	bool TryMovePieceDown();


public:
	void SetAccelerate(bool bAccelerate);
	void TryRotatePiece();
	void TryMovePiece(bool bRight);
};
