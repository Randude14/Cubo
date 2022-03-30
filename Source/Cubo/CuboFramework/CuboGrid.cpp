
#include "CuboGrid.h"

#include "CuboPiece.h"
#include "DrawDebugHelpers.h"
#include "ToolContextInterfaces.h"
#include "Kismet/GameplayStatics.h"

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
		GridScore = 0;
		ScoreChanged.Broadcast(this, GridScore, 0);

		HighlighterPiece = GetWorld()->SpawnActor<ACuboPiece>();
		HighlighterPiece->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		for(int i = 0; i < 4; i++)
		{
			ACuboBlock* Block = GetWorld()->SpawnActor<ACuboBlock>(HighlightBlockClass);
			Block->AttachToActor(HighlighterPiece, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			UMaterialInstanceDynamic* InstanceDynamic = UMaterialInstanceDynamic::Create(HighlightMaterial, this);
			Block->SetDefaultMaterial(InstanceDynamic);
		}

		HighlighterPiece->Init(false, 0.f, 0.f);

		RotateErrorPiece = GetWorld()->SpawnActor<ACuboPiece>();
		RotateErrorPiece->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

		for(int i = 0; i < 4; i++)
		{
			ACuboBlock* Block = GetWorld()->SpawnActor<ACuboBlock>(RotateErrorBlockClass);
			Block->AttachToActor(RotateErrorPiece, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}

		RotateErrorPiece->Init(false, 0.f, 0.f);
		RotateErrorPiece->SetActorHiddenInGame(true);
		
		Pause();
	}
}

// Called every frame
void ACuboGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(CurrentPiece == nullptr)
	{
		CurrentPiece = PieceQueue->PopPiece();
		CurrentPiece->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		float CheckLet = CurrentPiece->GetLeftOf(GetActorLocation());

		if(! FMath::IsNearlyZero(CheckLet))
		{
			CurrentPiece->AddPieceOffset(FVector(0.f, -CheckLet, 0.f));
		}

		CurrentPiece->AddPieceOffset(FVector(0.f, 0.f, -PieceQueue->PieceMoveInfo.BlockSpace * 3));

		UpdateHighlighter();
	}

	PieceMoveTimer -= DeltaTime;

	if(CurrentPiece && CurrentPiece->MoveTimerUp())
	{
		if(! TryMovePieceDown(CurrentPiece))
		{
			CurrentPiece = nullptr;
		}
		else
		{
			CurrentPiece->ResetMoveTimer();
		}
		UpdateHighlighter();
	}

	if(RotateErrorTimer < 0)
	{
		RotateErrorPiece->SetActorHiddenInGame(true);
	}
	else
	{
		RotateErrorTimer -= DeltaTime;
	}

	if(bShowGridSpaces)
	{
		TArray<int> Keys;
		Grid.GetKeys(Keys);

		for(int Index : Keys)
		{
			// int Index = GridWidth * Col + Row;

			int r = Index % GridWidth;
			int c = Index / GridWidth;
			
			FVector GridLocation = GetActorLocation();
			FVector IndexLocation = FVector(0.f, r * PieceQueue->PieceMoveInfo.BlockSpace, - c * PieceQueue->PieceMoveInfo.BlockSpace) + GridLocation;
			FVector IndexOutLocation = IndexLocation + FVector(-200.f, 0.f, 0.f);

			DrawDebugLine(GetWorld(), IndexLocation, IndexOutLocation, FColor::Red, false, -1, 0, 5.f);
		}
	}
}

void ACuboGrid::NewGame()
{
	SetActorTickEnabled(true);
	PieceQueue->EmptyQueue();

	TArray<int32> Indicies;
	Grid.GetKeys(Indicies);

	for(int Index : Indicies)
	{
		if(ACuboBlock* Block = Grid[Index])
		{
			Block->Destroy();
		}
	}
	Grid.Empty();
	GridScore = 0;
	ScoreChanged.Broadcast(this, 0, 0);
	bGamePaused = false;
}

void ACuboGrid::UpdateHighlighter()
{
	if(HighlighterPiece)
	{
		if(CurrentPiece)
		{
			TArray<ACuboBlock*> Blocks;
			CurrentPiece->GetBlocks(Blocks);

			TArray<ACuboBlock*> HighlightingBlocks;
			HighlighterPiece->GetBlocks(HighlightingBlocks);

			HighlighterPiece->SetActorLocation(CurrentPiece->GetActorLocation());

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
					
					if(UMaterialInstanceDynamic* Dynamic = Cast<UMaterialInstanceDynamic>(HBlock->GetDefaultMaterial()))
					{
						FLinearColor Color = Block->GetCubeColor();
						Dynamic->SetVectorParameterValue(FName("CubeColor"), Color);
					}
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

			float HDistance = FMath::Abs( HighlighterPiece->GetActorLocation().Z - CurrentPiece->GetActorLocation().Z );

			if(HDistance < HighlighterSpacing)
			{
				HighlighterPiece->SetActorHiddenInGame(true);
			}
			else
			{
				HighlighterPiece->SetActorLocation(Location);

				if(HighlighterPiece->IsHidden())
				{
					HighlighterPiece->SetActorHiddenInGame(false);
				}
			}
		}
		else if(! HighlighterPiece->IsHidden())
		{
			HighlighterPiece->SetActorHiddenInGame(true);
		}
	}
}

// Player controls

bool ACuboGrid::TryMovePieceRL(ACuboPiece* Piece, bool bRight)
{
	if(Piece && PieceQueue)
	{
		float MoveDistance = (bRight) ? PieceQueue->PieceMoveInfo.BlockSpace : -PieceQueue->PieceMoveInfo.BlockSpace;

		FVector CurrentLoc = Piece->GetActorLocation();
		Piece->AddPieceOffset(FVector(0.f, MoveDistance, 0.f));

		if(! IsPieceInLegalSpot(Piece))
		{
			// Move piece back if it's not legal
			Piece->SetActorLocation(CurrentLoc);
			return false;
		}
		else
		{
			UpdateHighlighter();
			return true;
		}
	}

	return false;
}

FCuboGridLocation ACuboGrid::LinePlaneIntersect(ACuboPiece* Piece, FVector Forward, FVector DragPointer)
{
	FVector DragForward = Forward + DragPointer * 3000.f;
	FVector GridForward = -GetActorForwardVector();
	FVector GridLocation = FMath::LinePlaneIntersection(Forward, DragForward, GetActorLocation(), GridForward);
	FVector RelativeGridLocation = GridLocation - GetActorLocation();
	return ConvertToGridSpace(RelativeGridLocation);
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

					if(PieceGridLocation.Y < 0)
					{
						Block->SetDefaultMaterial(OutOfBoundsMaterial);
					}
				}

				CheckFilledLines();

				Piece->Destroy();
			}
			return false;
		}
		return true;
	}
	return false;
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
		return true;
	}
	return false;
}

FCuboGridLocation ACuboGrid::ConvertToGridSpace(FVector Location)
{
	// invert the location's Z since the board goes down not up
	// Also need to add some "leeway" on the end to account for floating round errors
	Location.Z = (-Location.Z) + PieceQueue->PieceMoveInfo.GetMoveDistance() / 2.f;

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

void ACuboGrid::CheckFilledLines()
{
	TArray<int> FilledLineIndexes;
	
	for(int i = 0; i < GridHeight; i++)
	{
		bool bFilled = true;
		
		for(int j = 0; j < GridWidth; j++)
		{
			int Index = GridWidth * i + j;
			
			if(! Grid.Contains(Index))
			{
				bFilled = false;
				break;
			}
		}

		if(bFilled)
		{
			FilledLineIndexes.Add(i);
		}
	}

	if(FilledLineIndexes.Num())
	{
		if(ScoreByLines.Num() > FilledLineIndexes.Num()-1)
		{
			double Score = ScoreByLines[FilledLineIndexes.Num()-1];

			GridScore += Score;

			if(ScoreChanged.IsBound())
			{
				ScoreChanged.Broadcast(this, GridScore, Score);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Somehow filled more lines than what is possible? (%d)"), FilledLineIndexes.Num())
		}

		// start from the bottom
		for(int i = FilledLineIndexes.Num()-1; i >= 0 ; i--)
		{
			int Col = FilledLineIndexes[i];

			// delete row
			for(int Row = 0; Row < GridWidth; Row++)
			{
				int Index = GridWidth * Col + Row;
				if(Index)
				{
					ACuboBlock* Block = Grid[Index];
					if(Block)
					{
						Block->Destroy();
					}
					Grid.Remove(Index);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Invalid index detected while trying to clear lines (%d)"), Index)
				}
			}
			
			// Move up one row
			TArray<int> GridKeys;
			Grid.GetKeys(GridKeys);
			GridKeys.Sort();

			int Cutoff = GridWidth * Col;
			
			for(int j = GridKeys.Num()-1; j >= 0; j--)
			{
				int Index = GridKeys[j];
				
				// Don't care about blocks that are below the row we just deleted
				if(Index < Cutoff)
				{
					if(Grid.Contains(Index))
					{
						ACuboBlock* Block = Grid[Index];
						Grid.Add(Index+GridWidth, Block);
						FVector BlockLocation = FVector(0.f, 0.f, -PieceQueue->PieceMoveInfo.BlockSpace);
						Block->SetActorLocation( Block->GetActorLocation() + BlockLocation );

						Grid.Remove(Index);
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Invalid index detected while trying to move lines (%d)"), Index)
					}
				}
			}

			// Need to offset lines above since the rows have been shifted down
			for(int k = i-1; k >= 0; k--)
			{
				FilledLineIndexes[k] = FilledLineIndexes[k] + 1;
			}
		}
	}
}

bool ACuboGrid::TryRotatePiece(ACuboPiece* Piece)
{
	if(bGamePaused)
	{
		return false;
	}
	if(Piece)
	{
		if(Piece->CanRotate())
		{
			int CurrentRot = Piece->GetRotate();
			int Rotate = CurrentRot + 1;
			FVector CurrentLoc = Piece->GetActorLocation();

			if(Rotate == 4)
			{
				Rotate = 0;
			}
			Piece->SetRotate(Rotate);

			FVector RightBorder = GetActorLocation() + FVector(0.f, (PieceQueue->PieceMoveInfo.BlockSpace) * (GridWidth-1), 0.f);
			float CheckRight = Piece->GetRightOf(RightBorder);
			float CheckLeft = Piece->GetLeftOf(GetActorLocation());
		
			bool bIsRight = ! FMath::IsNearlyZero(CheckRight);
			bool bIsLeft = ! FMath::IsNearlyZero(CheckLeft);

			if(bIsRight && bIsLeft)
			{
				UE_LOG(LogTemp, Error, TEXT("Rotated piece somehow outside of bounds on both sides? GridWidth is either too low or piece is too wide."));
			}
			else if(bIsRight)
			{
				Piece->AddPieceOffset(FVector(0.f, -CheckRight, 0.f));
				
			}
			else if(bIsLeft)
			{
				Piece->AddPieceOffset(FVector(0.f, -CheckLeft, 0.f));
			}

			CurrentLoc = Piece->GetActorLocation();

			if(! IsPieceInLegalSpot(Piece))
			{
				if(RotateErrorPiece->IsHidden())
				{
					RotateErrorPiece->SetActorHiddenInGame(false);
				}

				RotateErrorTimer = RotateErrorVisibleTime;
			
				TArray<ACuboBlock*> Blocks;
				Piece->GetBlocks(Blocks);

				TArray<ACuboBlock*> RotateErrorBlocks;
				RotateErrorPiece->GetBlocks(RotateErrorBlocks);

				// match blocks of the current piece
				for(int i = 0; i < RotateErrorBlocks.Num(); i++)
				{
					ACuboBlock* REBlock = RotateErrorBlocks[i];
				
					if(i < RotateErrorBlocks.Num())
					{
						ACuboBlock* Block = Blocks[i];
						REBlock->SetActorHiddenInGame(false);
						REBlock->SetActorScale3D(Block->GetActorScale());
						REBlock->SetActorLocation(Block->GetActorLocation());
					}
					else
					{
						REBlock->SetActorHiddenInGame(true);
					}
				}
				
				// Move piece back if it is not in a legal spot
				Piece->SetActorLocation(CurrentLoc);
				Piece->SetRotate(CurrentRot);
				return false;
			}
			UpdateHighlighter();
			return true;
		}
	}
	return false;
}

bool ACuboGrid::IsBlockHere(int Row, int Col)
{
	int Index = GridWidth * Col + Row;
	return Grid.Find(Index) != nullptr;
}

bool ACuboGrid::IsBlockHere(FCuboGridLocation Pos)
{
	return IsBlockHere(Pos.X ,Pos.Y );
}

void ACuboGrid::SetBlock(int Row, int Col, ACuboBlock* Block)
{
	int Index = GridWidth * Col + Row;

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

	if(Block == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Block somehow null at (%d, %d) for Grid %s"), Row, Col, *GetName())
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


