// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CuboGrid.h"
#include "Cubo/UI/CuboMenuActor.h"
#include "CuboPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CUBO_API ACuboPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Cubo")
	void SetUseMotionControls(bool bMotionControls);
	
	UFUNCTION(BlueprintCallable, Category="Cubo")
	void SetLockPieceOnBoost(bool bLockBoost);

	UFUNCTION(BlueprintCallable, Category="Cubo")
	void SaveSettings();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Cubo")
	bool UseMotionControls();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Cubo")
	bool ShouldLockPieceOnBoost();
	
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void LoadSettings();
	
	ACuboGrid* GetOwningGrid() const;
	ACuboMenuActor* GetMenuActor() const;

	template<class T>
	T* GetActorOfClassFromWorld(UClass* Class);

protected:
	UPROPERTY()
	ACuboGrid* OwningGrid;
	UPROPERTY()
	ACuboMenuActor* MenuActor;

private:
	bool bUseMotionControls=true;
	bool bLockPieceOnBoost=true;
};
