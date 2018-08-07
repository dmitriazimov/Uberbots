#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AI/Navigation/NavMeshBoundsVolume.h"
#include "ActorDisplacer.generated.h"

// This is a terrible design that I have to use do displace the navmesh.
// I would intuitively derrive from ANavMeshBoundsVolume and make the custom behaviour over there
// but that makes the engine crash since ANavMeshBoundsVolume is not fully exposed. 
// The engine code will have to be changed to fix this, although I don't think this is a big enough problem to warrant that.

UCLASS()
class UBERBOTS_API AActorDisplacer : public AActor
{
	GENERATED_BODY()
	
public:	
	AActorDisplacer();

	UPROPERTY(EditAnywhere)
	ANavMeshBoundsVolume* nav_mesh_volume;

protected:
	virtual void BeginPlay() override;

private:

	FTimerHandle nav_mesh_position_updater;

private:

	void UpdateNavMesh();
};
