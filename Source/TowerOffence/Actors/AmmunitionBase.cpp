#include "AmmunitionBase.h"

#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/DamageType.h"

AAmmunitionBase::AAmmunitionBase() {
	PrimaryActorTick.bCanEverTick = false;
}

float AAmmunitionBase::GetFireRate() const {
	return FireRate;
}

void AAmmunitionBase::BeginPlay() {
	Super::BeginPlay();
}
