// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "CuboMainMenu.generated.h"

UCLASS()
class CUBO_API ACuboMainMenu : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACuboMainMenu();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cubo")
	USceneComponent* MenuRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cubo")
	UWidgetComponent* NewGame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cubo")
	UWidgetComponent* Settings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cubo")
	UWidgetComponent* QuitGame;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
