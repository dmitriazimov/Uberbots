#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Waypoint.generated.h"

UCLASS()
class UBERBOTS_API AWaypoint : public AActor
{
	GENERATED_BODY()
	
public:	

	AWaypoint();

	class UStaticMeshComponent* mesh;
};
