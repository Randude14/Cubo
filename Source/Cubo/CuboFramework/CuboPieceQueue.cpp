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
	int32 Piece = FMath::RandRange(0, GeneratedCuboPieces.Num()-1);

	FCuboPieceInfo CuboPieceInfo = GeneratedCuboPieces[Piece];
	FString Pattern = CuboPieceInfo.PiecePattern;
	
	if(! Pattern.IsEmpty())
	{
		if(ACuboPiece* Cubo = Cast<ACuboPiece>( GetWorld()->SpawnActor(CuboPieceClass) ))
		{
			Cubo->GetRootComponent()->SetMobility(EComponentMobility::Movable);
			Cubo->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			FVector AnchorPos = FVector::ZeroVector;
			bool bPieceShouldRotate = false;
		
			CuboPieces.Add(Cubo);

			TArray<ACuboBlock*> Blocks; 
		
			int r = 0, c = 0;
			
			for(int j = 0; j < Pattern.Len(); j++)
			{
				if(Pattern[j] == 'X' || Pattern[j] == 'A') // A is for anchor postion
				{
					if(ACuboBlock* CuboBlock = Cast<ACuboBlock>( GetWorld()->SpawnActor(CuboBlockClass) ))
					{
						CuboBlock->AttachToActor(Cubo, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
						CuboBlock->SetColor(CuboPieceInfo.CubeColor);
						CuboBlock->SetActorScale3D(BlockScale);
						FVector Location(0.f, r * PieceMoveInfo.BlockSpace, c * PieceMoveInfo.BlockSpace); // ignore X
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

			float AllocateSpace = (c + BlocksBetween) * PieceMoveInfo.BlockSpace;

			for(ACuboPiece* CuboPiece : CuboPieces)
			{
				CuboPiece->AddActorLocalOffset(FVector(0.f, 0.f, AllocateSpace));
			}
			
			
			Cubo->SetActorRelativeLocation( FVector::ZeroVector );
			Cubo->Init(bPieceShouldRotate);
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
		CuboPieces.RemoveAt(0);

		SpawnPiece();

		return Popped;
	}
	
	return nullptr;
}
