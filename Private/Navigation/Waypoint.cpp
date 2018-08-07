#include "Navigation/Waypoint.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

AWaypoint::AWaypoint()
{
	PrimaryActorTick.bCanEverTick = false;
	mesh = CreateDefaultSubobject < UStaticMeshComponent>(TEXT("mesh"));
	static ConstructorHelpers::FObjectFinder< UStaticMesh> mesh_ref(TEXT("StaticMesh'/Game/Geometry/Meshes/Waypoint'"));
	if (mesh_ref.Object != NULL)
	{
		mesh->SetStaticMesh(mesh_ref.Object);
		mesh->SetCollisionProfileName(TEXT("NoCollision"));
	}
	mesh->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));
	SetActorHiddenInGame(true);
	SetActorLocation(FVector(1207.0f, GetActorLocation().Y, GetActorLocation().Z));
}