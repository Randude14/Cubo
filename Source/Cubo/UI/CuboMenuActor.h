// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "CuboMenuActor.generated.h"

UCLASS()
class CUBO_API ACuboMenuActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACuboMenuActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cubo")
	USceneComponent* MenuRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cubo")
	UWidgetComponent* MainMenuComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	class UCuboMainMenu* MainMenu;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
