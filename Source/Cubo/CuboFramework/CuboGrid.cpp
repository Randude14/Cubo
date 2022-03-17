
#include "CuboGrid.h"
#include "CuboPiece.h"

ACuboGrid::ACuboGrid()
{
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
	SetRootComponent(RootSceneComponent);

	PieceQueueComponent = CreateDefaultSubobject<UChildActorComponent>("PieceQueue");
	PieceQueueComponent->SetupAttachment(RootSceneComponent);
	PieceQueueComponent->SetChildActorClass(ACuboPieceQueue::StaticClass());
	
	PrimaryActorTick.bCanEverTick = true;
}

void ACuboGrid::BeginPlay()
{
	Super::BeginPlay();

	PieceQueue = Cast<ACuboPieceQueue>( PieceQueueComponent->GetChildActor() );

	if(PieceQueue == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find ACuboPieceQueue for %s"), *GetName())
	}
}

// Called every frame
void ACuboGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(CurrentPiece == nullptr)
	{
		CurrentPiece = PieceQueue->PopPiece();
		CurrentPiece->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		float CheckLet = CurrentPiece->GetLeftOf(GetActorLocation());

		if(! FMath::IsNearlyZero(CheckLet))
		{
			CurrentPiece->AddPieceOffset(FVector(0.f, -CheckLet, 0.f));
		}
	}

	PieceMoveTimer -= DeltaTime;

	if(PieceMoveTimer < 0)
	{
		TryMovePieceDown();
	}
	
	Timer -= DeltaTime;

	if(Timer < 0.f)
	{
		Timer = SpawnTime;
	}
}

// Player controls

void ACuboGrid::TryMovePiece(bool bRight)
{
	if(CurrentPiece && PieceQueue)
	{
		float MoveDistance = (bRight) ? PieceQueue->PieceMoveInfo.BlockSpace : -PieceQueue->PieceMoveInfo.BlockSpace;
		CurrentPiece->AddPieceOffset(FVector(0.f, MoveDistance, 0.f));
	}
}

bool ACuboGrid::TryMovePieceDown()
{
	if(CurrentPiece)
	{
		FPieceMoveInfo PieceMoveInfo = PieceQueue->PieceMoveInfo;
		PieceMoveTimer += (bShouldAccelerate) ? PieceMoveInfo.AccelerateTime : PieceMoveInfo.NormalTime;
		CurrentPiece->AddPieceOffset(FVector(0.f, 0.f, -PieceMoveInfo.GetMoveDistance()));
		
		float Bottom = GetActorLocation().Z - PieceMoveInfo.BlockSpace * GridHeight;
		float PieceZ = CurrentPiece->GetActorLocation().Z;
		
		if(PieceZ <= Bottom)
		{
			CurrentPiece = nullptr;
		}
	}
	return true;
}

void ACuboGrid::TryRotatePiece()
{
	if(CurrentPiece)
	{
		int Rotate = CurrentPiece->GetRotate()+1;

		if(Rotate == 4)
		{
			Rotate = 0;
		}
		CurrentPiece->SetRotate(Rotate);

		FVector PieceLocation = GetActorLocation();
		float CheckLeft = CurrentPiece->GetLeftOf(GetActorLocation());

		if(! FMath::IsNearlyZero(CheckLeft))
		{
			CurrentPiece->AddPieceOffset(FVector(0.f, -CheckLeft, 0.f));
		}
	}
}

bool ACuboGrid::IsPieceHere(int Row, int Col)
{
	int Index = GridWidth * Row + Col;
	return Grid.Find(Index) != nullptr;
}

bool ACuboGrid::IsPieceHere(FVector2D Pos)
{
	return IsPieceHere( FMath::Floor(Pos.X) , FMath::Floor(Pos.Y) );
}

bool ACuboGrid::SetPiece(int Row, int Col, ACuboPiece* Piece)
{
	if(Piece)
	{
		if(! IsPieceHere(Row, Col))
		{
			int Index = GridWidth * Row + Col;
		}
	}
	return false;
}

void ACuboGrid::SetAccelerate(bool bAccelerate)
{
	bShouldAccelerate = bAccelerate;

	if(bAccelerate)
	{
		PieceMoveTimer -= PieceQueue->PieceMoveInfo.NormalTime - PieceQueue->PieceMoveInfo.AccelerateTime;
	}
}



