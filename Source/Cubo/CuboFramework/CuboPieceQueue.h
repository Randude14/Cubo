// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CuboPieceQueue.generated.h"

class ACuboPiece;
class ACuboBlock;

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
	UMaterialInstance* DefaultMaterial;

	// Color of this piece when it is spawned
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	UMaterialInstance* HighlightMaterial;
	
};


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


UCLASS()
class CUBO_API ACuboPieceQueue : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACuboPieceQueue();

	// The number of cubo pieces that are in the queue at any time
	UPROPERTY(EditAnywhere, Category="Cubo", meta=(ClampMin = "1", ClampMax = "8", UIMin = "1", UIMax = "8"))
	int32 PieceQueueSize=3;

	// The number of block spaces between cubo pieces
	UPROPERTY(EditAnywhere, Category="Cubo", meta=(ClampMin = "1", ClampMax = "8", UIMin = "1", UIMax = "8"))
	int32 BlocksBetween=3;
	
	// List of the cubo pieces that will be generated and spawned for the player to fit in the grid
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	TArray<FCuboPieceInfo> GeneratedCuboPieces;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	TSubclassOf<ACuboPiece> CuboPieceClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	TSubclassOf<ACuboBlock> CuboBlockClass;

	// Contains information about the block spacing and move distancews
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	FPieceMoveInfo PieceMoveInfo;

	// The scale of spawned cubo blocks
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	FVector BlockScale;

	void EmptyQueue();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<ACuboPiece*> CuboPieces;


	void SpawnPiece();
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ACuboPiece* PopPiece();
};
