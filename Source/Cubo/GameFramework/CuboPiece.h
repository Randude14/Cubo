// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CuboPiece.generated.h"

UCLASS()
class CUBO_API ACuboPiece : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACuboPiece();

	void Init(float MoveBy, float TimeBetween);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
