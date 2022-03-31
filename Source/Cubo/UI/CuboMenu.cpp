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
			UCuboWindow* Widget = Cast<UCuboWindow>( CreateWidget(GetOwningPlayer(), Class, FName(WindowID)) );

			if(Widget)
			{
				Widget->WindowOpenRequest.AddDynamic(this, &UCuboMenu::OnWindowOpenRequest);
				Widget->WindowReturn.AddDynamic(this, &UCuboMenu::UCuboMenu::OnWindowReturn);
				WindowWidgets.Add(WindowID, Widget);
				MenuWidgetSwitcher->AddChild(Widget);
			}
		}
	}
}

void UCuboMenu::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	TArray<FString> WindowIDs;
	WindowWidgets.GetKeys(WindowIDs);
	
	for(FString ID : WindowIDs)
	{
		UCuboWindow* Window = WindowWidgets[ID];
		Window->ReleaseSlateResources(bReleaseChildren);
		Window = nullptr;
	}
	
	WindowWidgets.Empty();
	if(MenuWidgetSwitcher)
	{
		MenuWidgetSwitcher->ReleaseSlateResources(bReleaseChildren);
	}
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
			FString PrevWindow = WindowStack[0];
			ShowWindow(PrevWindow);
		}
		else
		{
			CloseWindow();
		}
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

void UCuboMenu::CloseWindow()
{
	SetVisibility(ESlateVisibility::Hidden);
	WindowStack.Empty();
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
