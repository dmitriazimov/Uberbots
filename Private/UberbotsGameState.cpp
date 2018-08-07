#include "UberbotsGameState.h"

AUberbotsGameState::AUberbotsGameState()
{
    ProjectileManagers = CreateDefaultSubobject<UAllProjectileManagers>(TEXT("AllProjectileManagers"));
}

void AUberbotsGameState::FireProjectile(const TArray<AActor*>& ShooterActorsToIgnore, const FTransform& InitialTransform, const FVector& Direction, TSubclassOf<AProjectileManager> ProjectileClass)
{
    AProjectileManager* Manager = ProjectileManagers->GetManager(ProjectileClass);
    if (Manager == nullptr) return;

    Manager->Fire(ShooterActorsToIgnore, InitialTransform, Direction);
}