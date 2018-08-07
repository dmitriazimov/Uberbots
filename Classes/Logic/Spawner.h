#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UberbotsCharacter.h"
#include "Spawner.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerSpawn, AUberbotsCharacter*, spawned_character);

UCLASS()
class UBERBOTS_API ASpawner : public AActor
{
	GENERATED_BODY()

public:

	ASpawner();

protected:

	virtual void BeginPlay() override;

private:

	void RespwanAfterFall();
	void ControlPlayerDistancing();
	void RepositionFromTo(AActor* from, FVector to);
	void PreventPlayersFromGoingBackwards();
	FTimerHandle distancing_control_handle;

	const int min_players = 2;
	const int max_players = 4;
	const float side_scroll_X = 1207.0f;
	const float max_acceptable_distance = 1500.0f;
	const float default_spawn_height = 1100.0f;
	const float left_deviation = 600.0f;

	float current_y_progress;
	float fall_limit;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AUberbotsCharacter* SpawnPlayer(int player_id);
	void SpawnPlayers(int amount); // temporary

	UFUNCTION()
	void WhenPlayerSpawns(AUberbotsCharacter* spawned_character);

public:
	UPROPERTY(BlueprintAssignable, Category = "Event Dispatchers")
	FOnPlayerSpawn PlayerSpawn;
};