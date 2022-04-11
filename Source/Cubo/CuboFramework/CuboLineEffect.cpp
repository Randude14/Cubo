#include "CuboLineEffect.h"

ACuboLineEffect::ACuboLineEffect()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACuboLineEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ParticleLifetime -= DeltaTime;

	if(ParticleLifetime <= 0.f)
	{
		Destroy();
	}
}

