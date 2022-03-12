// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboGrid.h"


// Sets default values
ACuboGrid::ACuboGrid()
{
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
	SetRootComponent(RootSceneComponent);
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACuboGrid::BeginPlay()
{
	Super::BeginPlay();
	Timer = SpawnTime;
}

// Called every frame
void ACuboGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for(USceneComponent* Cubo : Cubos)
	{
		FVector CurrentLoc = Cubo->GetComponentLocation();
		CurrentLoc.Z -= DeltaTime*100;

		Cubo->SetWorldLocation(CurrentLoc);
	}
	
	Timer -= DeltaTime;

	if(Timer < 0.f)
	{
		Timer = SpawnTime;

		if(GeneratedCuboPieces.Num())
		{
			int32 Piece = FMath::RandRange(0, GeneratedCuboPieces.Num()-1);

			FCuboPieceInfo CuboPieceInfo = GeneratedCuboPieces[Piece];

			FString Pattern = CuboPieceInfo.PiecePattern;
			USceneComponent* Cubo = NewObject<USceneComponent>(this, USceneComponent::StaticClass());
			Cubo->SetupAttachment(RootSceneComponent);
			Cubo->RegisterComponent();
			Cubo->SetRelativeLocation(FVector::ZeroVector);
			Cubos.Add(Cubo);

			if(! Pattern.IsEmpty())
			{
				int r = 0, c = 0;
				for(int i = 0; i < Pattern.Len(); i++)
				{
					if(Pattern[i] == 'X')
					{
						ACuboBlock* CuboBlock = NewObject<ACuboBlock>(this, CuboBlockClass);
						CuboBlock->AttachToComponent(Cubo, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
						CuboBlock->RegisterAllComponents();
						CuboBlock->SetColor(CuboPieceInfo.CubeColor);
						FVector Location(r*100.f, 0, c*100.f);
						r++;

						CuboBlock->SetActorRelativeLocation(Location);

						int32 H = FMath::RandRange(0, 1);
						if(H == 1)
						{
							CuboBlock->Highlight();
						}
					}
					else if(Pattern[i] == ' ')
					{
						// new row, reset row and move to next column
						r = 0;
						c++;
					}
					else
					{
						r++;
					}
				}
			}
		}
	}
}

