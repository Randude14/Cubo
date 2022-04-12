#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UCuboNumberSpinner.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCuboSpinnerValueChanged, const float&, SpinnerValue);

UCLASS()
class CUBO_API UCuboNumberSpinner : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta=(BindWidget), Category="Cubo")
	UButton* PlusButton;

	UPROPERTY(EditAnywhere, meta=(BindWidget), Category="Cubo")
	UButton* MinusButton;

	UPROPERTY(EditAnywhere, meta=(BindWidget), Category="Cubo")
	UTextBlock* SpinnerLabel;

	UPROPERTY(EditAnywhere, meta=(BindWidget), Category="Cubo")
	float SpinnerMax=2.0f;

	UPROPERTY(EditAnywhere, meta=(BindWidget), Category="Cubo")
	float SpinnerMin=0.0f;

	UPROPERTY(EditAnywhere, meta=(BindWidget), Category="Cubo")
	float SpinnerStep=0.1f;

	UPROPERTY(BlueprintAssignable, Category="Cubo")
	FCuboSpinnerValueChanged SpinnerValueChanged;

	UPROPERTY(EditAnywhere, meta=(BindWidget), Category="Cubo")
	FText SpinnerLabelFormat = FText::FromString("{0}");

	UFUNCTION(BlueprintCallable, Category="Cubo")
	void SetSpinnerValue(float Value);

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void PlusButtonPressed();
	UFUNCTION()
	void MinusButtonPressed();

	void UpdateSpinnerLabel();

	float SpinnerValue;
};
