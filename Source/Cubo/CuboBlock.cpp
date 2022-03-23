// Fill out your copyright notice in the Description page of Project Settings.


#include "CuboBlock.h"

ACuboBlock::ACuboBlock()
{
}

// Called when the game starts or when spawned
void ACuboBlock::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACuboBlock::Highlight()
{
	if(GetStaticMeshComponent() && HighlightMaterial)
	{
		GetStaticMeshComponent()->SetMaterial(0, HighlightMaterial);
	}
}

void ACuboBlock::Unhighlight()
{
	if(GetStaticMeshComponent() && DefaultMaterial)
	{
		GetStaticMeshComponent()->SetMaterial(0, DefaultMaterial);
	}
}

void ACuboBlock::SetDefaultMaterial(UMaterialInstance* Material)
{
	DefaultMaterial = Material;
	Unhighlight();
}

UMaterialInstance* ACuboBlock::GetDefaultMaterial() const
{
	return DefaultMaterial;
}

void ACuboBlock::SetHighlightMaterial(UMaterialInstance* HMaterial)
{
	HighlightMaterial = HMaterial;
}

FLinearColor ACuboBlock::GetCubeColor() const
{
	if(DefaultMaterial)
	{
		FHashedMaterialParameterInfo ParameterInfo(FName("CubeColor"));
		FLinearColor CubeColor;

		DefaultMaterial->GetVectorParameterValue(ParameterInfo, CubeColor);
		return CubeColor;
	}

	return FLinearColor::Transparent;
}
