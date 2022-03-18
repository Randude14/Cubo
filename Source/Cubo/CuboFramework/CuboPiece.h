// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cubo/CuboBlock.h"
#include "GameFramework/Actor.h"
#include "CuboPiece.generated.h"

UCLASS()
class CUBO_API ACuboPiece : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACuboPiece();

	void Init(bool bShouldRotate);

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* AnchorComponent;

protected:

	UPROPERTY(EditAnywhere)
	USceneComponent* RootSceneComponent;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	TArray<ACuboBlock*> Blocks;
	int Rotate = 0;
	bool bInit = false;
	bool bAccelerating = false;
	bool bRotateEnabled;
	float MoveTimer;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Highlight();
	void Unhighlight();

	int GetRotate();
	void SetRotate(int Rot);
	bool CanRotate() { return bRotateEnabled; }
	
	FVector GetDownDirection();
	FVector GetRightDirection();
	float GetLeftOf(FVector Location);
	float GetRightOf(FVector Location);
	float GetTotalHeight();
	float GetHeightBelowAnchor();
	float GetHeightAboveAnchor();

	void GetBlocks(TArray<ACuboBlock*>&);

	void AddPieceOffset(FVector Offset);
};
