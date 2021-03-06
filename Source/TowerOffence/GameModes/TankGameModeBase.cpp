#include "TankGameModeBase.h"

#include "TowerOffence/Actors/ForceFieldBase.h"
#include "TowerOffence/Pawns/PawnBase.h"
#include "TowerOffence/Pawns/TurretBase.h"
#include "TowerOffence/Pawns/MineBase.h"
#include "TowerOffence/Controllers/PlayerControllerBase.h"

#include "Kismet/GameplayStatics.h"

ATankGameModeBase::ATankGameModeBase() {}

void ATankGameModeBase::ActorDied(AActor* DeadActor) {
	if (DeadActor == PlayerTank) {
		PlayerTank->HandleDestruction();
		HandleGameOver(false);

		if (PlayerController) {
			PlayerController->SetPlayerEnabledState(false);
		}
	} else if (ATurretBase* DestroyedTurret = Cast<ATurretBase>(DeadActor)) {
		DestroyedTurret->HandleDestruction();
		TargetTurretsCount -= 1;

		if (TargetTurretsCount == 0) {
			HandleGameOver(true);
		}
	} else if (AMineBase* DestroyedMine = Cast<AMineBase>(DeadActor)) {
		DestroyedMine->BlowUp();
	} else if (AForceFieldBase* DestroyedForceField = Cast<AForceFieldBase>(DeadActor)) {
		DestroyedForceField->Deactivate();
	}
}

void ATankGameModeBase::BeginPlay() {
	Super::BeginPlay();
	HandleGameStart();
}

void ATankGameModeBase::HandleGameStart() {
	TArray<AActor*> TurretActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATurretBase::StaticClass(), TurretActors);
	TargetTurretsCount = TurretActors.Num();
	PlayerTank = Cast<APawnBase>(UGameplayStatics::GetPlayerPawn(this, 0));
	PlayerController = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));

	GameStart();
}

void ATankGameModeBase::HandleGameOver(bool PlayerWon) {
	GameOver(PlayerWon);
}
