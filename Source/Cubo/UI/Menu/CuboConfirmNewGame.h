// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CuboConfirmNewGame.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class CUBO_API UCuboConfirmNewGame : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category="Cubo")
	UButton* NewGameConfirmButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category="Cubo")
	UButton* NewGameDenyButton;
};
