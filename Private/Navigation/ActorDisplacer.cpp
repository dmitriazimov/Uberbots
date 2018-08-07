#include "Navigation/ActorDisplacer.h"
#include "TimerManager.h"
#include "Logic/StaticFunctionLibrary.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Components/SceneComponent.h"

AActorDisplacer::AActorDisplacer()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AActorDisplacer::BeginPlay()
{
	Super::BeginPlay();
	if (nav_mesh_volume)
	{
		GetWorldTimerManager().SetTimer(nav_mesh_position_updater, this, &AActorDisplacer::UpdateNavMesh, 0.5f, true, 0.5f);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find a nav mesh bounds volume"));
	}
}

void AActorDisplacer::UpdateNavMesh()
{
	nav_mesh_volume->SetActorLocation(FVector(1207.0f, StaticFunctionLibrary::GetAveragePlayerPosition(GetWorld()).Y, nav_mesh_volume->GetActorLocation().Z));
	GetWorld()->GetNavigationSystem()->OnNavigationBoundsUpdated(nav_mesh_volume);
}

