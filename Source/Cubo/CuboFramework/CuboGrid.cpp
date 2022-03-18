
#include "CuboGrid.h"
#include "CuboPiece.h"
#include "FileCache.h"

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
	else
	{
		TinyPieceOffset = PieceQueue->PieceMoveInfo.BlockSpace / 1.f;
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
		if(! TryMovePieceDown())
		{
			CurrentPiece = nullptr;
		}
	}
}

// Player controls

void ACuboGrid::TryMovePiece(bool bRight)
{
	if(CurrentPiece && PieceQueue)
	{
		float MoveDistance = (bRight) ? PieceQueue->PieceMoveInfo.BlockSpace : -PieceQueue->PieceMoveInfo.BlockSpace;

		FVector CurrentLoc = CurrentPiece->GetActorLocation();
		CurrentPiece->AddPieceOffset(FVector(0.f, MoveDistance, 0.f));

		if(! IsPieceInLegalSpot())
		{
			// Move piece back if it's not legal
			CurrentPiece->SetActorLocation(CurrentLoc);
		}
	}
}

bool ACuboGrid::TryMovePieceDown()
{
	if(CurrentPiece)
	{
		FPieceMoveInfo PieceMoveInfo = PieceQueue->PieceMoveInfo;
		PieceMoveTimer += (bShouldAccelerate) ? PieceMoveInfo.AccelerateTime : PieceMoveInfo.NormalTime;
		FVector CurrentLoc = CurrentPiece->GetActorLocation();
		CurrentPiece->AddPieceOffset(FVector(0.f, 0.f, -PieceMoveInfo.GetMoveDistance()));
		
		if(! IsPieceInLegalSpot())
		{
			CurrentPiece->SetActorLocation(CurrentLoc);

			TArray<ACuboBlock*> Blocks;
			CurrentPiece->GetBlocks(Blocks);

			for(ACuboBlock* Block : Blocks)
			{
				FVector Relative = Block->GetActorLocation() - GetActorLocation();
				Relative.Z = FMath::Abs( Relative.Z ) + TinyPieceOffset;

				FVector2D PieceGridLocation;
				PieceGridLocation.X = FMath::RoundToInt(Relative.Y / PieceQueue->PieceMoveInfo.BlockSpace);
				PieceGridLocation.Y = FMath::FloorToInt(Relative.Z / PieceQueue->PieceMoveInfo.BlockSpace);

				SetBlock(PieceGridLocation, Block);
				Block->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				Block->Unhighlight();
			}

			CurrentPiece->Destroy();
			return false;
		}
	}
	return true;
}

bool ACuboGrid::IsPieceInLegalSpot()
{
	if(CurrentPiece)
	{
		TArray<ACuboBlock*> Blocks;
		CurrentPiece->GetBlocks(Blocks);

		for(ACuboBlock* Block : Blocks)
		{
			FVector Relative = Block->GetActorLocation() - GetActorLocation();
			Relative.Z = FMath::Abs( Relative.Z ) + TinyPieceOffset;

			FVector2D PieceGridLocation;
			PieceGridLocation.X = FMath::RoundToInt(Relative.Y / PieceQueue->PieceMoveInfo.BlockSpace);
			PieceGridLocation.Y = FMath::CeilToInt(Relative.Z / PieceQueue->PieceMoveInfo.BlockSpace);

			if(IsBlockHere(PieceGridLocation) || PieceGridLocation.Y > GridHeight )
			{
				return false;
			}
		}
	}
	return true;
}

void ACuboGrid::TryRotatePiece()
{
	if(CurrentPiece)
	{
		if(CurrentPiece->CanRotate())
		{
			int CurrentRot = CurrentPiece->GetRotate();
			int Rotate = CurrentRot + 1;
			FVector CurrentLoc = CurrentPiece->GetActorLocation();

			if(Rotate == 4)
			{
				Rotate = 0;
			}
			CurrentPiece->SetRotate(Rotate);

			float CheckLeft = CurrentPiece->GetLeftOf(GetActorLocation());

			if(! FMath::IsNearlyZero(CheckLeft))
			{
				CurrentPiece->AddPieceOffset(FVector(0.f, -CheckLeft, 0.f));
				CurrentLoc = CurrentPiece->GetActorLocation();
			}

			if(! IsPieceInLegalSpot())
			{
				// Move piece back if it is not in a legal spot
				CurrentPiece->SetActorLocation(CurrentLoc);
				CurrentPiece->SetRotate(CurrentRot);
			}
		}
	}
}

bool ACuboGrid::IsBlockHere(int Row, int Col)
{
	int Index = GridHeight * Row + Col;
	return Grid.Find(Index) != nullptr;
}

bool ACuboGrid::IsBlockHere(FVector2D Pos)
{
	return IsBlockHere( FMath::Floor(Pos.X) , FMath::Floor(Pos.Y) );
}

void ACuboGrid::SetBlock(int Row, int Col, ACuboBlock* Block)
{
	int Index = GridHeight * Row + Col;

	if(Index > GridWidth * GridHeight)
	{
		UE_LOG(LogTemp, Warning, TEXT("Position (%d, %d) is out of bounds for Grid %s (%d, %d)"), Row, Col, *GetName(), GridWidth, GridHeight)
		return;
	}
	
	if(Grid.Contains(Index))
	{
		UE_LOG(LogTemp, Warning, TEXT("There is already a block at (%d, %d) for Grid %s"), Row, Col, *GetName())
		return;
	}

	Grid.Add(Index, Block);
}

void ACuboGrid::SetBlock(FVector2D Pos, ACuboBlock* Block)
{
	SetBlock( FMath::Floor(Pos.X) , FMath::Floor(Pos.Y), Block );
}

void ACuboGrid::SetAccelerate(bool bAccelerate)
{
	bShouldAccelerate = bAccelerate;

	if(bAccelerate)
	{
		PieceMoveTimer -= PieceQueue->PieceMoveInfo.NormalTime - PieceQueue->PieceMoveInfo.AccelerateTime;
	}
}



