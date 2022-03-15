// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboGrid.h"
#include "CuboPiece.h"

// Sets default values
ACuboGrid::ACuboGrid()
{
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
	SetRootComponent(RootSceneComponent);
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BlockScale = FVector(1.f);
}

// Called when the game starts or when spawned
void ACuboGrid::BeginPlay()
{
	Super::BeginPlay();
	//Timer = SpawnTime;
}

// Called every frame
void ACuboGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PieceMoveTimer -= DeltaTime;

	if(PieceMoveTimer < 0)
	{
		TryMovePieceDown();
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
			

			if(! Pattern.IsEmpty())
			{
				if(ACuboPiece* Cubo = Cast<ACuboPiece>( GetWorld()->SpawnActor(CuboPieceClass) ))
				{
					Cubo->GetRootComponent()->SetMobility(EComponentMobility::Movable);
					Cubo->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
					FVector AnchorPos = FVector::ZeroVector;
					bool bPieceShouldRotate = false;
				
					Cubos.Add(Cubo);

					TArray<ACuboBlock*> Blocks; 
				
					int r = 0, c = 0;
					for(int i = 0; i < Pattern.Len(); i++)
					{
						if(Pattern[i] == 'X' || Pattern[i] == 'A') // A is for anchor postion
						{
							if(ACuboBlock* CuboBlock = Cast<ACuboBlock>( GetWorld()->SpawnActor(CuboBlockClass) ))
							{
								CuboBlock->AttachToActor(Cubo, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
								CuboBlock->SetColor(CuboPieceInfo.CubeColor);
								CuboBlock->SetActorScale3D(BlockScale);
								FVector Location(0.f, r * PieceMoveInfo.BlockSpace, c * PieceMoveInfo.BlockSpace); // ignore X
								r++;

								if(Pattern[i] == 'A')
								{
									AnchorPos = Location;
									bPieceShouldRotate = true;
								}
						
								CuboBlock->SetActorRelativeLocation(Location);
								Blocks.Add(CuboBlock);

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

					if(bPieceShouldRotate)
					{
						Cubo->AddActorWorldOffset(AnchorPos);

						for(ACuboBlock* CuboBlock : Blocks)
						{
							CuboBlock->AddActorLocalOffset(-AnchorPos);
						}
					}
				

					Cubo->Init(bPieceShouldRotate);
					if(CurrentPiece == nullptr)
					{
						CurrentPiece = Cubo;
					}
				}
			}
		}
	}
}

// Player controls

void ACuboGrid::TryMovePiece(bool bRight)
{
	if(CurrentPiece)
	{
		float MoveDistance = (bRight) ? PieceMoveInfo.BlockSpace : -PieceMoveInfo.BlockSpace;
		CurrentPiece->AddPieceOffset(FVector(0.f, MoveDistance, 0.f));
	}
}

bool ACuboGrid::TryMovePieceDown()
{
	if(CurrentPiece)
	{
		PieceMoveTimer += (bShouldAccelerate) ? PieceMoveInfo.AccelerateTime : PieceMoveInfo.NormalTime;
		CurrentPiece->AddPieceOffset(FVector(0.f, 0.f, -PieceMoveInfo.GetMoveDistance()));
		
		float Bottom = GetActorLocation().Z - PieceMoveInfo.BlockSpace * GridHeight;
		float PieceZ = CurrentPiece->GetActorLocation().Z;
		
		if(PieceZ <= Bottom)
		{
			if(Cubos.Num() > 1)
			{
				Cubos.Remove(CurrentPiece);
				CurrentPiece = Cubos[0];
			}
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
			Grid.Add(Index, Piece);
		}
	}
	return false;
}

void ACuboGrid::SetAccelerate(bool bAccelerate)
{
	bShouldAccelerate = bAccelerate;

	if(bAccelerate)
	{
		PieceMoveTimer -= PieceMoveInfo.NormalTime - PieceMoveInfo.AccelerateTime;
	}
}



