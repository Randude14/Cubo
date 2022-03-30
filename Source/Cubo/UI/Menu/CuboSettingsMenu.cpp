#include "CuboSettingsMenu.h"
#include "Components/Button.h"

void UCuboSettingsMenu::NativeOnInitialized()
{
	if(BackButton)
	{
		BackButton->OnReleased.AddDynamic(this, &UCuboSettingsMenu::CloseSettings);
	}
}

void UCuboSettingsMenu::SaveSettings()
{
	
}

void UCuboSettingsMenu::CloseSettings()
{
	WindowReturn.Broadcast();
}


