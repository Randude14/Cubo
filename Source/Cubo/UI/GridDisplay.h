// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "GridDisplay.generated.h"

UCLASS()
class CUBO_API AGridDisplay : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGridDisplay();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootSceneComponent;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* DisplayComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
