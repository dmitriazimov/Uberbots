#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Inventory/AllProjectileManagers.h"
#include "UberbotsGameState.generated.h"

UCLASS()
class UBERBOTS_API AUberbotsGameState : public AGameState
{
	GENERATED_BODY()

public:
    AUberbotsGameState();

    UAllProjectileManagers* GetProjectileManagers() const { return ProjectileManagers; }

    UFUNCTION(BlueprintCallable)
    void FireProjectile(const TArray<AActor*>& ShooterActorsToIgnore, const FTransform& InitialTransform, const FVector& Direction, TSubclassOf<AProjectileManager> ProjectileClass);

protected:
    UPROPERTY()
    UAllProjectileManagers* ProjectileManagers = nullptr;
};