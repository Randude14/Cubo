#pragma once

#include "Cubo/UI/CuboWindow.h"
#include "CuboMainMenu.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class CUBO_API UCuboMainMenu : public UCuboWindow
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category="Cubo")
	UButton* NewGameButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category="Cubo")
	UButton* SettingsButton;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category="Cubo")
	UButton* QuitGameButton;

	virtual void NativeOnInitialized() override;

private:

	UFUNCTION()
	void NewGamePressed();
	UFUNCTION()
	void SettingsPressed();
	UFUNCTION()
	void QuitGamePressed();
};