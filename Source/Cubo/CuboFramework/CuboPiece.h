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

	friend class ACuboGrid;
	friend class ACuboPieceQueue;

public:
	// Sets default values for this actor's properties
	ACuboPiece();

	void Init(bool bShouldRotate, float NTime, float ATime);

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* AnchorComponent;

protected:

	UPROPERTY(EditAnywhere)
	USceneComponent* RootSceneComponent;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<ACuboBlock*> Blocks;
	UPROPERTY()
	AActor* GrippingCurrently;
	
	int Rotate = 0;
	bool bInit = false;
	bool bAccelerating = false;
	bool bRotateEnabled;
	float MoveTimer;

	float NormalTime;
	float AccelerateTime;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Highlight();
	void Unhighlight();

	void GrabPiece(AActor* Gripping);
	void ReleasePiece(AActor* Gripping);
	bool IsBeingGrabbed() const;

	void SetAccelerate(bool bAccelerate);
	bool MoveTimerUp() { return MoveTimer <= 0.f; };
	void ResetMoveTimer();
	
protected:

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

	void SetActorHiddenInGame(bool bNewHidden) override;
};
