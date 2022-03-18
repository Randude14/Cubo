
#include "CuboGrid.h"

#include "CuboPiece.h"
#include "DrawDebugHelpers.h"

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
		TinyPieceOffset = 1.f / PieceQueue->PieceMoveInfo.BlockSpace;

		HighlighterPiece = GetWorld()->SpawnActor<ACuboPiece>();
		HighlighterPiece->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		for(int i = 0; i < 4; i++)
		{
			ACuboBlock* Block = GetWorld()->SpawnActor<ACuboBlock>(HighlightBlockClass);
			Block->AttachToActor(HighlighterPiece, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}

		HighlighterPiece->Init(false);
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

		UpdateHighlighter();
	}

	PieceMoveTimer -= DeltaTime;

	if(PieceMoveTimer < 0)
	{
		PieceMoveTimer += (bShouldAccelerate) ? PieceQueue->PieceMoveInfo.AccelerateTime : PieceQueue->PieceMoveInfo.NormalTime;
		
		if(! TryMovePieceDown(CurrentPiece))
		{
			CurrentPiece = nullptr;
		}
	}

	if(bShowGridSpaces)
	{
		TArray<int> Keys;
		Grid.GetKeys(Keys);

		for(int Index : Keys)
		{
			// int Index = GridHeight * Row + Col;

			int i = Index;
			int r = 0;
			while(i > GridHeight) {r++; i-= GridHeight;}
			int c = Index - r * GridHeight;

			FVector GridLocation = GetActorLocation();
			FVector IndexLocation = FVector(0.f, r * PieceQueue->PieceMoveInfo.BlockSpace, - c * PieceQueue->PieceMoveInfo.BlockSpace) + GridLocation;
			FVector IndexOutLocation = IndexLocation + FVector(-200.f, 0.f, 0.f);

			DrawDebugLine(GetWorld(), IndexLocation, IndexOutLocation, FColor::Red, true, -1, 0, 5.f);
		}
	}
}

void ACuboGrid::UpdateHighlighter()
{
	if(HighlighterPiece)
	{
		if(CurrentPiece)
		{
			if(HighlighterPiece->IsHidden())
			{
				HighlighterPiece->SetActorHiddenInGame(false);
			}
			
			HighlighterPiece->SetActorHiddenInGame(false);
			TArray<ACuboBlock*> Blocks;
			CurrentPiece->GetBlocks(Blocks);

			TArray<ACuboBlock*> HighlightingBlocks;
			HighlighterPiece->GetBlocks(HighlightingBlocks);

			// match blocks of the current piece
			for(int i = 0; i < HighlightingBlocks.Num(); i++)
			{
				ACuboBlock* HBlock = HighlightingBlocks[i];
				
				if(i < HighlightingBlocks.Num())
				{
					ACuboBlock* Block = Blocks[i];
					HBlock->SetActorHiddenInGame(false);
					HBlock->SetActorScale3D(Block->GetActorScale());
					HBlock->SetActorLocation(Block->GetActorLocation());
					HBlock->SetColor(Block->GetColor());
				}
				else
				{
					HBlock->SetActorHiddenInGame(true);
				}
			}

			FVector Location = HighlighterPiece->GetActorLocation();
			
			while( TryMovePieceDown(HighlighterPiece, false) )
			{
				Location = HighlighterPiece->GetActorLocation();
			}
			
			HighlighterPiece->SetActorLocation(Location);
		}
		else if(! HighlighterPiece->IsHidden())
		{
			HighlighterPiece->SetActorHiddenInGame(true);
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

		if(! IsPieceInLegalSpot(CurrentPiece))
		{
			// Move piece back if it's not legal
			CurrentPiece->SetActorLocation(CurrentLoc);
		}
		else
		{
			UpdateHighlighter();
		}
	}
}

bool ACuboGrid::TryMovePieceDown(ACuboPiece* Piece, bool bSpawnBlocks)
{
	if(Piece)
	{
		FPieceMoveInfo PieceMoveInfo = PieceQueue->PieceMoveInfo;
		FVector CurrentLoc = Piece->GetActorLocation();
		Piece->AddPieceOffset(FVector(0.f, 0.f, -PieceMoveInfo.GetMoveDistance()));
		
		if(! IsPieceInLegalSpot(Piece))
		{
			if(bSpawnBlocks)
			{
				Piece->SetActorLocation(CurrentLoc);

				TArray<ACuboBlock*> Blocks;
				Piece->GetBlocks(Blocks);
				FVector GridLocation = GetActorLocation();

				for(ACuboBlock* Block : Blocks)
				{
					FVector BlockLocation = Block->GetActorLocation();
					FVector Relative = BlockLocation - GridLocation;
				
					FCuboGridLocation PieceGridLocation = ConvertToGridSpace(Relative);

					SetBlock(PieceGridLocation, Block);
				}

				Piece->Destroy();
			}
			return false;
		}
	}
	return true;
}

bool ACuboGrid::IsPieceInLegalSpot(ACuboPiece* Piece)
{
	if(Piece)
	{
		TArray<ACuboBlock*> Blocks;
		Piece->GetBlocks(Blocks);
		if(Blocks.Num() == 0)
		{
			return false;
		}
		int BlockSpacing = FMath::RoundToInt(PieceQueue->PieceMoveInfo.BlockSpace);

		for(ACuboBlock* Block : Blocks)
		{
			FVector Relative = Block->GetActorLocation() - GetActorLocation();

			FCuboGridLocation PieceGridLocation = ConvertToGridSpace(Relative);

			if(IsBlockHere(PieceGridLocation) || PieceGridLocation.Y > (GridHeight-1) || (PieceGridLocation.X < 0 || PieceGridLocation.X >= GridWidth)  )
			{
				return false;
			}
			
			int BlockZ = FMath::RoundToInt(Relative.Z);

			// check spacing above in case of rotation pieces and side movements
			if(BlockZ % BlockSpacing != 0)
			{
				PieceGridLocation.Y -= 1;
				
				if(IsBlockHere(PieceGridLocation))
				{
					return false;
				}
			}
		}
	}
	return true;
}

FCuboGridLocation ACuboGrid::ConvertToGridSpace(FVector Location)
{
	Location.Z = FMath::Abs( Location.Z ) + PieceQueue->PieceMoveInfo.GetMoveDistance() / 2.f;

	float RawGridY = Location.Z / PieceQueue->PieceMoveInfo.BlockSpace;
	FCuboGridLocation GridSpaceLocation;
	GridSpaceLocation.X = FMath::RoundToInt(Location.Y / PieceQueue->PieceMoveInfo.BlockSpace);
	GridSpaceLocation.Y = FMath::FloorToInt( RawGridY );

	float RoundedOff = RawGridY - FMath::FloorToInt(RawGridY);

	if(RoundedOff > PieceQueue->PieceMoveInfo.MovePieceBy)
	{
		GridSpaceLocation.Y += 1;
	}

	return GridSpaceLocation;
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

			FVector RightBorder = GetActorLocation() + FVector(0.f, (PieceQueue->PieceMoveInfo.BlockSpace) * (GridWidth-1), 0.f);
			float CheckRight = CurrentPiece->GetRightOf(RightBorder);
			float CheckLeft = CurrentPiece->GetLeftOf(GetActorLocation());
		
			bool bIsRight = ! FMath::IsNearlyZero(CheckRight);
			bool bIsLeft = ! FMath::IsNearlyZero(CheckLeft);

			if(bIsRight && bIsLeft)
			{
				UE_LOG(LogTemp, Error, TEXT("Rotated piece somehow outside of bounds on both sides? GridWidth is either too low or piece is too wide."));
			}
			else if(bIsRight)
			{
				CurrentPiece->AddPieceOffset(FVector(0.f, -CheckRight, 0.f));
				
			}
			else if(bIsLeft)
			{
				CurrentPiece->AddPieceOffset(FVector(0.f, -CheckLeft, 0.f));
			}

			CurrentLoc = CurrentPiece->GetActorLocation();

			if(! IsPieceInLegalSpot(CurrentPiece))
			{
				// Move piece back if it is not in a legal spot
				CurrentPiece->SetActorLocation(CurrentLoc);
				CurrentPiece->SetRotate(CurrentRot);
			}
			else
			{
				UpdateHighlighter();
			}
		}
	}
}

bool ACuboGrid::IsBlockHere(int Row, int Col)
{
	int Index = GridHeight * Row + Col;
	return Grid.Find(Index) != nullptr;
}

bool ACuboGrid::IsBlockHere(FCuboGridLocation Pos)
{
	return IsBlockHere(Pos.X ,Pos.Y );
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
	Block->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	Block->Unhighlight();
}

void ACuboGrid::SetBlock(FCuboGridLocation Pos, ACuboBlock* Block)
{
	SetBlock(Pos.X , Pos.Y, Block );
}

void ACuboGrid::SetAccelerate(bool bAccelerate)
{
	bShouldAccelerate = bAccelerate;

	if(bAccelerate)
	{
		PieceMoveTimer -= PieceQueue->PieceMoveInfo.NormalTime - PieceQueue->PieceMoveInfo.AccelerateTime;
	}
}



