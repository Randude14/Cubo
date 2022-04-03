// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboPlayerController.h"

#include "Kismet/GameplayStatics.h"

void ACuboPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	OwningGrid = GetActorOfClassFromWorld<ACuboGrid>(ACuboGrid::StaticClass());
	MenuActor = GetActorOfClassFromWorld<ACuboMenuActor>(ACuboMenuActor::StaticClass());
	LoadSettings();
}

void ACuboPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	MenuActor = nullptr;
}


template <class T>
T* ACuboPlayerController::GetActorOfClassFromWorld(UClass* Class)
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, Class, Actors);

	if(Actors.Num())
	{
		T* ResultActor = Cast<T>(Actors[0]);
		return ResultActor;
	}
	return nullptr;
}

ACuboGrid* ACuboPlayerController::GetOwningGrid() const
{
	return OwningGrid;
}

ACuboMenuActor* ACuboPlayerController::GetMenuActor() const
{
	return MenuActor;
}

void ACuboPlayerController::LoadSettings()
{
	FString JsonString = "";

#if WITH_EDITOR
	FString PlayerSettingsFile = FPaths::ProjectDir() / TEXT("Settings") / TEXT("PlayerSettings-project.json");
#else
	FString PlayerSettingsFile = FPaths::ProjectDir() / TEXT("Settings") / TEXT("PlayerSettings-packaged.json");
#endif
	
	FFileHelper::LoadFileToString(JsonString, *PlayerSettingsFile);
	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create( JsonString );
	TSharedPtr<FJsonObject> Object;
	bool bSuccessful = FJsonSerializer::Deserialize(Reader, Object);

	if(bSuccessful)
	{
		TMap<FString, TSharedPtr<FJsonValue>> Values = Object->Values;

		if(Values.Contains("motion-controls"))
		{
			TSharedPtr<FJsonValue> MotionControlsJson = Values["motion-controls"];

			bool bValue = false;
			if(MotionControlsJson->TryGetBool(bValue))
			{
				bUseMotionControls = bValue;
			}
		}
		if(Values.Contains("lock-boost"))
		{
			TSharedPtr<FJsonValue> LockBoostJson = Values["lock-boost"];

			bool bValue = false;
			if(LockBoostJson->TryGetBool(bValue))
			{
				bLockPieceOnBoost = bValue;
			}
		}
	}
	else
	{
		SaveSettings();
	}
}

void ACuboPlayerController::SaveSettings()
{
#if WITH_EDITOR
	FString PlayerSettingsFile = FPaths::ProjectDir() / TEXT("Settings") / TEXT("PlayerSettings-project.json");
#else
	FString PlayerSettingsFile = FPaths::ProjectDir() / TEXT("Settings") / TEXT("PlayerSettings-packaged.json");
#endif

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	
	TSharedPtr<FJsonValue> MotionControlsSetting = MakeShareable(new FJsonValueBoolean(bUseMotionControls));
	TSharedPtr<FJsonValue> LockBoostSetting = MakeShareable(new FJsonValueBoolean(bLockPieceOnBoost));

	JsonObject->SetField("motion-controls", MotionControlsSetting);
	JsonObject->SetField("lock-boost", LockBoostSetting);

	FString OutputString = "";
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer, true);

	FFileHelper::SaveStringToFile(OutputString, *PlayerSettingsFile);
}

bool ACuboPlayerController::ShouldLockPieceOnBoost()
{
	return bLockPieceOnBoost;
}

bool ACuboPlayerController::UseMotionControls()
{
	return bUseMotionControls;
}

void ACuboPlayerController::SetUseMotionControls(bool bMotionControls)
{
	bUseMotionControls = bMotionControls;
}

void ACuboPlayerController::SetLockPieceOnBoost(bool bLockBoost)
{
	bLockPieceOnBoost = bLockBoost;
}

