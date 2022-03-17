
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

float ACuboPiece::GetRightOf(FVector Location)
{
	float CheckRight = 0.f;
	
	for(ACuboBlock* Block : Blocks)
	{
		FVector RelativeLocation = Block->GetActorLocation() - Location;
		
		CheckRight = FMath::Max(CheckRight, RelativeLocation.Y);
	}

	return CheckRight;
}

float ACuboPiece::GetTotalHeight()
{
	float MinZ = 0.f;
	float MaxZ = 0.f;
	FVector Location = GetActorLocation();
	
	for(ACuboBlock* Block : Blocks)
	{
		FVector RelativeLocation = Block->GetActorLocation() - Location;
		
		MinZ = FMath::Min(MinZ, RelativeLocation.Z);
		MaxZ = FMath::Max(MaxZ, RelativeLocation.Z);
	}

	return MaxZ - MinZ;
}

float ACuboPiece::GetHeightBelowAnchor()
{
	float MinZ = 0.f;
	FVector Location = GetActorLocation();
	
	for(ACuboBlock* Block : Blocks)
	{
		FVector RelativeLocation = Block->GetActorLocation() - Location;
		
		MinZ = FMath::Min(MinZ, RelativeLocation.Z);
	}

	return FMath::Abs(MinZ);
}

float ACuboPiece::GetHeightAboveAnchor()
{
	float MaxZ = 0.f;
	FVector Location = GetActorLocation();
	
	for(ACuboBlock* Block : Blocks)
	{
		FVector RelativeLocation = Block->GetActorLocation() - Location;
		
		MaxZ = FMath::Max(MaxZ, RelativeLocation.Z);
	}

	return MaxZ;
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