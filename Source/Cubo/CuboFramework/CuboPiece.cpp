// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboPiece.h"

#include "DrawDebugHelpers.h"


// Sets default values
ACuboPiece::ACuboPiece()
{
	PrimaryActorTick.bCanEverTick = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
	SetRootComponent(RootSceneComponent);

	AnchorComponent = CreateDefaultSubobject<UStaticMeshComponent>("Anchor");
	AnchorComponent->SetupAttachment(RootSceneComponent);
}

void ACuboPiece::Init(bool bShouldRotate)
{
	this->bRotateEnabled = bShouldRotate;

	AnchorComponent->SetVisibility(bShouldRotate);
	
	TArray<AActor*> ChildActors;
	GetAttachedActors(ChildActors, false);
	
	for(AActor* Actor : ChildActors)
	{
		if(ACuboBlock* Block = Cast<ACuboBlock>(Actor))
		{
			Blocks.Add(Block);
		}
	}

	if(Blocks.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("%s spawned with zero blocks. How did this happen?"), *GetName())
	}
	else
	{
		bInit = true;
	}
}

// Called when the game starts or when spawned
void ACuboPiece::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACuboPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bInit)
	{
	}
}

void ACuboPiece::AddPieceOffset(FVector Offset)
{
	FVector PieceLocation = GetActorLocation() + Offset;
	SetActorLocation(PieceLocation);	
}

float ACuboPiece::GetLeftOf(FVector Location)
{
	float CheckLeft = 0.f;
	
	for(ACuboBlock* Block : Blocks)
	{
		FVector RelativeLocation = Block->GetActorLocation() - Location;
		
		CheckLeft = FMath::Min(CheckLeft, RelativeLocation.Y);
	}

	return CheckLeft;
}

FVector ACuboPiece::GetDownDirection()
{
	switch(Rotate)
	{
		case 0: return -GetActorUpVector(); break;
		case 1: return GetActorRightVector(); break;
		case 2: return GetActorUpVector(); break;
		case 3: return -GetActorRightVector(); break;
		default: return FVector::ZeroVector; break;
	}
}

FVector ACuboPiece::GetRightDirection()
{
	switch(Rotate)
	{
		case 0: return GetActorRightVector(); break;
		case 1: return GetActorUpVector(); break;
		case 2: return -GetActorRightVector(); break;
		case 3: return -GetActorUpVector(); break;
		default: return FVector::ZeroVector; break;
	}	
}

int ACuboPiece::GetRotate()
{
	return Rotate;
}

void ACuboPiece::SetRotate(int Rot)
{
	Rotate = FMath::Clamp(Rot, 0, 3);
	FRotator Rotation;
	Rotation.Roll = Rotate * 90;
	SetActorRelativeRotation(Rotation);
}

void ACuboPiece::Highlight()
{
	for(ACuboBlock* Block : Blocks)
	{
		Block->Highlight();
	}
}

void ACuboPiece::Unhighlight()
{
	for(ACuboBlock* Block : Blocks)
	{
		Block->Unhighlight();
	}
}