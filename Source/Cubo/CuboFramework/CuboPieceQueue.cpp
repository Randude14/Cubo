// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboPieceQueue.h"

#include "CuboPiece.h"

ACuboPieceQueue::ACuboPieceQueue()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACuboPieceQueue::BeginPlay()
{
	Super::BeginPlay();
	
	for(int i = 0; i < PieceQueueSize; i++)
	{
		SpawnPiece();
	}
}

void ACuboPieceQueue::SpawnPiece()
{
	if(GeneratedCuboPieces.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No generated cubo pieces found for ACuboPieceQueue '%s'"), *GetName());
	}
	int32 ChosenPiece = FMath::RandRange(0, GeneratedCuboPieces.Num()-1);

	FCuboPieceInfo CuboPieceInfo = GeneratedCuboPieces[ChosenPiece];
	FString Pattern = CuboPieceInfo.PiecePattern;
	
	
	if(! Pattern.IsEmpty())
	{
		if(ACuboPiece* Cubo = Cast<ACuboPiece>( GetWorld()->SpawnActor(CuboPieceClass) ))
		{
			Cubo->GetRootComponent()->SetMobility(EComponentMobility::Movable);
			Cubo->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			FVector AnchorPos = FVector::ZeroVector;
			bool bPieceShouldRotate = false;

			TArray<ACuboBlock*> Blocks; 
		
			int r = 0, c = 0;
			
			for(int j = 0; j < Pattern.Len(); j++)
			{
				if(Pattern[j] == 'X' || Pattern[j] == 'A') // A is for anchor postion
				{
					if(ACuboBlock* CuboBlock = Cast<ACuboBlock>( GetWorld()->SpawnActor(CuboBlockClass) ))
					{
						CuboBlock->AttachToActor(Cubo, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
						CuboBlock->SetDefaultMaterial(CuboPieceInfo.DefaultMaterial);
						CuboBlock->SetHighlightMaterial(CuboPieceInfo.HighlightMaterial);
						CuboBlock->SetActorScale3D(BlockScale);
						FVector Location(0.f, r * PieceMoveInfo.BlockSpace, - c * PieceMoveInfo.BlockSpace); // ignore X
						r++;

						if(Pattern[j] == 'A')
						{
							AnchorPos = Location;
							bPieceShouldRotate = true;
						}
				
						CuboBlock->SetActorRelativeLocation(Location);
						Blocks.Add(CuboBlock);

					}
				}
				else if(Pattern[j] == ' ')
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

			// If an anchor was found, we want the anchor in the same spot but to offset the blocks
			if(bPieceShouldRotate)
			{
				Cubo->AddActorWorldOffset(AnchorPos);

				for(ACuboBlock* CuboBlock : Blocks)
				{
					CuboBlock->AddActorLocalOffset(-AnchorPos);
				}
			}

			if(CuboPieces.Num() > 1)
			{
				ACuboPiece* PieceBefore = CuboPieces.Last();
				
				float RelativeHeight =  PieceBefore->GetActorLocation().Z - GetActorLocation().Z;
				float LastPieceHeight = ((BlocksBetween+1) * PieceMoveInfo.BlockSpace) + RelativeHeight;
				Cubo->SetActorRelativeLocation( FVector(0.f, 0.f, LastPieceHeight) );
			}

			CuboPieces.Add(Cubo);
			float CheckLeft = Cubo->GetLeftOf(GetActorLocation());
			Cubo->AddPieceOffset(FVector(0.f, CheckLeft, 0.f));
			Cubo->Init(bPieceShouldRotate, PieceMoveInfo.NormalTime, PieceMoveInfo.AccelerateTime);
		}
	}
}


void ACuboPieceQueue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ACuboPiece* ACuboPieceQueue::PopPiece()
{
	if(CuboPieces.Num() > 0)
	{
		ACuboPiece* Popped = CuboPieces[0];

		if(CuboPieces.Num() > 1)
		{
			ACuboPiece* NextPiece = CuboPieces[1];
			float MoveUpBy = GetActorLocation().Z - NextPiece->GetActorLocation().Z;
			FVector MoveVector = FVector(0.f, 0.f, MoveUpBy);
			NextPiece->AddPieceOffset(MoveVector);
				
			for(int i = 2; i < CuboPieces.Num(); i++)
			{
				ACuboPiece* Piece = CuboPieces[i];
				Piece->AddPieceOffset(MoveVector);
			}
		}
		
		CuboPieces.RemoveAt(0);

		SpawnPiece();

		return Popped;
	}
	
	return nullptr;
}
