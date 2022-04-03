// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboMenu.h"
#include "NetworkMessage.h"

#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"


void UCuboMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<FString> IDs;
	WindowClasses.GetKeys(IDs);

	for(FString WindowID : IDs)
	{
		TSubclassOf<UUserWidget> Class = WindowClasses[WindowID];

		if(Class)
		{
			UCuboWindow* Widget = CreateWidget<UCuboWindow>(GetOwningPlayer(), Class, FName(WindowID));

			if(Widget)
			{
				Widget->WindowOpenRequest.AddDynamic(this, &UCuboMenu::OnWindowOpenRequest);
				Widget->WindowReturn.AddDynamic(this, &UCuboMenu::OnWindowReturn);
				WindowWidgets.Add(WindowID, Widget);
				MenuWidgetSwitcher->AddChild(Widget);
			}
		}
	}
}

void UCuboMenu::NativeDestruct()
{
	Super::NativeDestruct();

	UE_LOG(LogTemp, Warning, TEXT("NativeDestruct"));
}


void UCuboMenu::DestroyWindows()
{
	if(MenuWidgetSwitcher)
	{
		while(MenuWidgetSwitcher->GetChildrenCount() > 0)
		{
			if(UWidget* Widget = MenuWidgetSwitcher->GetChildAt(MenuWidgetSwitcher->GetChildrenCount()-1))
			{
				Widget->RemoveFromParent();
			}
		}
	}

	WindowWidgets.Empty();
}

void UCuboMenu::OnWindowOpenRequest(const FString& WindowName)
{
	ShowWindow(WindowName);
}

void UCuboMenu::OnWindowReturn()
{
	if(WindowStack.Num())
	{
		WindowStack.Pop();

		if(WindowStack.Num())
		{
			FString PrevWindow = WindowStack.Pop();
			ShowWindow(PrevWindow);
		}
		else
		{
			CloseWindow(true);
		}
	}
	else
	{
		CloseWindow(true);
	}
}

void UCuboMenu::ShowWindow(FString ID)
{
	if(MenuWidgetSwitcher && WindowWidgets.Contains(ID))
	{
		if(UUserWidget* Window = WindowWidgets[ID])
		{
			SetVisibility(ESlateVisibility::Visible);
			MenuWidgetSwitcher->SetActiveWidget(Window);
			WindowStack.Add(ID);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find window for id '%s'"), *ID)
	}
}

bool UCuboMenu::CloseWindow(bool bForceClose)
{
	if(IsVisible())
	{
		if(UCuboWindow* Window = Cast<UCuboWindow>( MenuWidgetSwitcher->GetActiveWidget() ))
		{
			if(!Window->CanForceClose() && !bForceClose)
			{
				return false;
			}	
		}
	}
	
	SetVisibility(ESlateVisibility::Hidden);
	WindowStack.Empty();
	return true;
}

bool UCuboMenu::CanForceClose()
{
	if(IsVisible())
	{
		if(UCuboWindow* Window = Cast<UCuboWindow>(MenuWidgetSwitcher->GetActiveWidget()))
		{
			return Window->CanForceClose();
		}
	}
	return true;
}
