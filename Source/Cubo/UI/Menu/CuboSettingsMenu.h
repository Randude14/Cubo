// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cubo/UI/CuboWindow.h"
#include "CuboSettingsMenu.generated.h"

/**
 * 
 */
UCLASS()
class CUBO_API UCuboSettingsMenu : public UCuboWindow
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category="Cubo")
	class UButton* BackButton;

	UFUNCTION(BlueprintCallable, Category="Cubo")
	void SaveSettings();

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void CloseSettings();
};
