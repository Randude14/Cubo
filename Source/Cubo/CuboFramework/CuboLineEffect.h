#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CuboLineEffect.generated.h"

UCLASS()
class CUBO_API ACuboLineEffect : public AActor
{
	GENERATED_BODY()

public:
	ACuboLineEffect();

	UPROPERTY(EditAnywhere, Category="Cubo")
	float ParticleLifetime=0.5f;

public:
	virtual void Tick(float DeltaTime) override;
};
