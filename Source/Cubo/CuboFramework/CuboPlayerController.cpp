// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboPlayerController.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void ACuboPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	OwningGrid = GetActorOfClassFromWorld<ACuboGrid>(ACuboGrid::StaticClass());
	MenuActor = GetActorOfClassFromWorld<ACuboMenuActor>(ACuboMenuActor::StaticClass());
	GameSoundActor = GetActorOfClassFromWorld<AAmbientSound>(AAmbientSound::StaticClass());
	LoadSettings();
	if(GameSoundActor)
	{
		GameSoundActor->GetAudioComponent()->AdjustVolume(0.f, GameSoundVolume);
	}
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

			MotionControlsJson->TryGetBool(bUseMotionControls);
		}
		if(Values.Contains("lock-boost"))
		{
			TSharedPtr<FJsonValue> LockBoostJson = Values["lock-boost"];
			LockBoostJson->TryGetBool(bLockPieceOnBoost);
		}
		if(Values.Contains("game-volume"))
		{
			TSharedPtr<FJsonValue> LockBoostJson = Values["game-volume"];

			LockBoostJson->TryGetNumber(GameSoundVolume);
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
	TSharedPtr<FJsonValue> GameVolumeSetting = MakeShareable(new FJsonValueNumber(GameSoundVolume));

	JsonObject->SetField("motion-controls", MotionControlsSetting);
	JsonObject->SetField("lock-boost", LockBoostSetting);
	JsonObject->SetField("game-volume", GameVolumeSetting);

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

float ACuboPlayerController::GetGameSoundVolume() const
{
	return GameSoundVolume;
}

void ACuboPlayerController::SetUseMotionControls(bool bMotionControls)
{
	bUseMotionControls = bMotionControls;
}

void ACuboPlayerController::SetLockPieceOnBoost(bool bLockBoost)
{
	bLockPieceOnBoost = bLockBoost;
}

void ACuboPlayerController::SetGameVolume(float Volume)
{
	bool bShouldPlay = FMath::IsNearlyZero(GameSoundVolume) || GameSoundVolume <= 0.f;
	
	GameSoundVolume = Volume;
	if(GameSoundActor)
	{
		if(bShouldPlay && ! FMath::IsNearlyZero(GameSoundVolume) )
		{
			GameSoundActor->GetAudioComponent()->Play();
		}
		GameSoundActor->GetAudioComponent()->AdjustVolume(0.f, GameSoundVolume);
	}
}

