// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Delegates/Delegate.h"
#include "CuboMainMenu.generated.h"

class UWidgetSwitcher;
class UButton;
class UTextBlock;

enum EMenuSummonReason
{
	BEGINNING,
	PAUSE,
	GAMEOVER
};

UCLASS()
class CUBO_API UCuboMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category="Cubo")
	UWidgetSwitcher* MenuWidgetSwitcher;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category="Cubo")
	UButton* NewGameButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category="Cubo")
	UButton* ResumeGameButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category="Cubo")
	UButton* SettingsButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category="Cubo")
	UButton* QuitGameButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category="Cubo")
	UButton* NewGameConfirmButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category="Cubo")
	UButton* NewGameDenyButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category="Cubo")
	UTextBlock* GameOverLabel;

	// callback to the cubo menu actor so we can close this menu
	FSimpleMulticastDelegate OnMenuClosed;

	// Index in the widget switcher for the main menu
	UPROPERTY(EditAnywhere, Category="Cubo")
	int32 MainIndex;

	// Index in the widget switcher for the settings menu
	UPROPERTY(EditAnywhere, Category="Cubo")
	int32 SettingsIndex;

	// Index in the widget switcher for the confirm new game window
	UPROPERTY(EditAnywhere, Category="Cubo")
	int32 ConfirmNewGameIndex;

	virtual void NativeOnInitialized() override;

private:
	UPROPERTY()
	class ACuboGrid* Grid;

private:
	UFUNCTION()
	void NewGamePressed();
	UFUNCTION()
	void QuitGamePressed();
	UFUNCTION()
	void ResumeGamePressed();
	UFUNCTION()
	void SettingsPressed();
	UFUNCTION()
	void ConfirmNewGamePressed();
	UFUNCTION()
	void DenyNewGamePressed();
};
