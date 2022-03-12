// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboBlock.h"

ACuboBlock::ACuboBlock()
{
	ColorParam = FName("CubeColor");
	HighlightParam = FName("CubeHighlight");
}

// Called when the game starts or when spawned
void ACuboBlock::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACuboBlock::Highlight()
{
	if(DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(HighlightParam, HighlightPercentage);
	}
}

void ACuboBlock::Unhighlight()
{
	if(DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(HighlightParam, 0.f);
	}
}

void ACuboBlock::SetColor(const FLinearColor& CubeColor)
{
	if(! DynamicMaterial && GetStaticMeshComponent())
	{
		UMaterialInterface* MaterialInterface = GetStaticMeshComponent()->GetMaterial(0);

		if(MaterialInterface)
		{
			DynamicMaterial = UMaterialInstanceDynamic::Create(MaterialInterface, this, FName("CubeMaterial"));

			if(DynamicMaterial)
			{
				GetStaticMeshComponent()->SetMaterial(0, DynamicMaterial);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to create a UMaterialInstanceDynamic for %s."), *GetName())
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to set color. No material found for %s."), *GetName())
		}
	}
	
	if(DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue(ColorParam, CubeColor);
		Color = CubeColor;
	}
}
