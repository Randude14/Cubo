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

	UFUNCTION(BlueprintCallable, Category="Cubo")
	void ShowScreen(FString ID);

	UFUNCTION(BlueprintCallable, Category="Cubo")
	void CloseScreen();

	UFUNCTION(BlueprintCallable, Category="Cubo")
	bool CanForceClose();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnVisibilityChanged(ESlateVisibility Visibility);

	UPROPERTY()
	class UCuboMenu* MenuScreen;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
