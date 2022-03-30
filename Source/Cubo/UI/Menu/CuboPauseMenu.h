#pragma once

#include "CoreMinimal.h"
#include "CuboMainMenu.h"
#include "Cubo/UI/CuboWindow.h"
#include "UObject/Object.h"
#include "CuboPauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class CUBO_API UCuboPauseMenu : public UCuboMainMenu
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget), Category="Cubo")
	class UButton* ResumeButton;

	virtual void NativeOnInitialized() override;
	
private:
	UFUNCTION()
	void ResumeButtonPressed();
};
