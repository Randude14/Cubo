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
	
	void Highlight();
	void Unhighlight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void SetDefaultMaterial(UMaterialInstance* Material);
	UMaterialInstance* GetDefaultMaterial() const;
	void SetHighlightMaterial(UMaterialInstance* HMaterial);

	FLinearColor GetCubeColor() const;

private:
	UPROPERTY()
	UMaterialInstance* DefaultMaterial;
	UPROPERTY()
	UMaterialInstance* HighlightMaterial;
};
