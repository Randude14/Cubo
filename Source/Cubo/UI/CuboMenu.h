
#pragma once

#include "CoreMinimal.h"
#include "CuboWindow.h"

#include "CuboMenu.generated.h"

class UWidgetSwitcher;
class UImage;

class UCuboMainMenu;
class UCuboSettingsMenu;
class UCuboConfirmNewGame;

UCLASS()
class CUBO_API UCuboMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category="Cubo")
	UWidgetSwitcher* MenuWidgetSwitcher;

	// Map of the window classes, map string ids to classes
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	TMap<FString, TSubclassOf<UCuboWindow>> WindowClasses;
	
	void ShowWindow(FString ID);
	void CloseWindow();
	bool CanForceClose();

	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY()
	TMap<FString, UCuboWindow*> WindowWidgets;

	TArray<FString> WindowStack;

	UFUNCTION()
	void OnWindowOpenRequest(const FString& WindowName);

	UFUNCTION()
	void OnWindowReturn();
};
