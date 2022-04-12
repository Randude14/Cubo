#include "UCuboNumberSpinner.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UCuboNumberSpinner::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(PlusButton)
	{
		PlusButton->OnClicked.AddDynamic(this, &UCuboNumberSpinner::PlusButtonPressed);
	}
	if(MinusButton)
	{
		MinusButton->OnClicked.AddDynamic(this, &UCuboNumberSpinner::MinusButtonPressed);
	}
	SetSpinnerValue(SpinnerMin);
}

void UCuboNumberSpinner::PlusButtonPressed()
{
	SetSpinnerValue(SpinnerValue + SpinnerStep);
}

void UCuboNumberSpinner::MinusButtonPressed()
{
	SetSpinnerValue(SpinnerValue - SpinnerStep);
}

void UCuboNumberSpinner::UpdateSpinnerLabel()
{
	FText Label = FText::Format(SpinnerLabelFormat, FText::AsNumber(SpinnerValue));

	SpinnerLabel->SetText(Label);
}

void UCuboNumberSpinner::SetSpinnerValue(float Value)
{
	SpinnerValue = FMath::Clamp(Value, SpinnerMin, SpinnerMax);

	UpdateSpinnerLabel();
	SpinnerValueChanged.Broadcast(SpinnerValue);
}



