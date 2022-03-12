// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cubo/CuboBlock.h"
#include "GameFramework/Actor.h"
#include "CuboGrid.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	TSubclassOf<ACuboBlock> CuboBlockClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	int32 GridWidth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	int32 GridHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	float SpawnTime = 5.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	float Timer = 0.f;

	TArray<USceneComponent*> Cubos;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
