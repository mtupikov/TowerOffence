#include "ProjectileBase.h"

#include "Camera/CameraShake.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystemComponent.h"

#include "Kismet/GameplayStatics.h"

AProjectileBase::AProjectileBase() {
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	TrailParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail Particle"));
	TrailParticle->SetupAttachment(RootComponent);
}

float AProjectileBase::GetFireRate() const {
	return FireRate;
}

void AProjectileBase::BeginPlay() {
	Super::BeginPlay();

	UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	GetWorld()->GetTimerManager().SetTimer(LifeSpanTimerHandle, this, &AProjectileBase::DestroyProjectile, LifeSpanTime, false);
}

void AProjectileBase::OnHit(
	UPrimitiveComponent*,
	AActor* OtherActor,
	UPrimitiveComponent*,
	FVector,
	const FHitResult&
) {
	const auto* MyOwner = GetOwner();
	if (!MyOwner) {
		return;
	}

	if (!OtherActor || OtherActor == this || OtherActor == MyOwner) {
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(LifeSpanTimerHandle);

	UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, DamageType);
	GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(CameraHitShake);

	DestroyProjectile();
}

void AProjectileBase::DestroyProjectile() {
	UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, GetActorLocation());

	Destroy();
}
