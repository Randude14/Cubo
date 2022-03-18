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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	FName ColorParam;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	FName HighlightParam;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	float HighlightPercentage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UMaterialInstanceDynamic* DynamicMaterial;
	FLinearColor Color;

public:	
	UFUNCTION(BlueprintCallable)
	void SetColor(const FLinearColor& CubeColor);

	FLinearColor GetColor() const;
};
