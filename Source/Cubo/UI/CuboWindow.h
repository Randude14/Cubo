// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CuboWindow.generated.h"

// This is used as a callback to the CuboMenu to open a new window
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCuboWindowOpenRequest, const FString&, WindowName);

// This is used as a callback to the CuboMenu to go back in the window return
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCuboWindowReturn);

UCLASS()
class CUBO_API UCuboWindow : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Cubo")
	FCuboWindowOpenRequest WindowOpenRequest;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Cubo")
	FCuboWindowReturn WindowReturn;

	bool CanForceClose() const
	{
		return bCanForceClose;
	}

protected:
	// Indicates that the user can press the menu to close this window
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cubo")
	bool bCanForceClose = true;
};
