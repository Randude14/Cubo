// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cubo/UI/CuboWindow.h"
#include "CuboConfirmNewGame.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class CUBO_API UCuboConfirmNewGame : public UCuboWindow
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category="Cubo")
	UButton* NewGameConfirmButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category="Cubo")
	UButton* NewGameDenyButton;

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void ConfirmPressed();
	UFUNCTION()
	void DenyPressed();
};
