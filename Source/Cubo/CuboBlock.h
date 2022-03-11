// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "CuboBlock.generated.h"

UCLASS()
class CUBO_API ACuboBlock : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACuboBlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UMaterialInstanceDynamic* DynamicMaterial;

public:	
	UFUNCTION(BlueprintCallable)
	void SetColor(const FLinearColor& CubeColor);
};
