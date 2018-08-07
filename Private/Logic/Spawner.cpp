// Fill out your copyright notice in the Description page of Project Settings.

#include "Logic/Spawner.h"
#include "Engine.h"
#include "Logic/StaticFunctionLibrary.h"
#include "Math/NumericLimits.h"
#include "Kismet/GameplayStatics.h"
#include "UberbotsCharacter.h"

// Sets default values
ASpawner::ASpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	PlayerSpawn.AddDynamic(this, &ASpawner::WhenPlayerSpawns);
	GetWorldTimerManager().SetTimer(distancing_control_handle, this, &ASpawner::ControlPlayerDistancing, 0.5f, true, 0.5f);
	current_y_progress = StaticFunctionLibrary::FindLeftMostPlayerProgress(GetWorld()) + left_deviation;
	fall_limit = 0.0f;
}

void ASpawner::RespwanAfterFall()
{
	for (TActorIterator<AUberbotsCharacter> living_characters_iter(GetWorld()); living_characters_iter; ++living_characters_iter)
	{
		if (living_characters_iter->GetActorLocation().Z < fall_limit)
		{
			living_characters_iter->SetActorLocation(FVector(living_characters_iter->GetActorLocation().X, living_characters_iter->GetActorLocation().Y, default_spawn_height));
			living_characters_iter->GetCharacterMovement()->Velocity = FVector(0, 0, 0);
		}
	}
}

void ASpawner::ControlPlayerDistancing()
{
	for (TActorIterator<AUberbotsCharacter> living_characters_iter(GetWorld()); living_characters_iter; ++living_characters_iter)
	{
		for (TActorIterator<AUberbotsCharacter> living_characters_iter_other(GetWorld()); living_characters_iter_other; ++living_characters_iter_other)
		{
			if (FMath::Abs((*living_characters_iter)->GetActorLocation().Y - (*living_characters_iter_other)->GetActorLocation().Y) > max_acceptable_distance)
			{
				if ((*living_characters_iter)->GetActorLocation().Y - (*living_characters_iter_other)->GetActorLocation().Y > 0)
				{
					RepositionFromTo((*living_characters_iter_other), (*living_characters_iter)->GetActorLocation());
				}
				else
				{
					RepositionFromTo((*living_characters_iter), (*living_characters_iter_other)->GetActorLocation());
				}
				break;
			}
		}
	}
}

void ASpawner::RepositionFromTo(AActor * from, FVector to)
{
	from->SetActorLocation(FVector(to.X, to.Y, default_spawn_height));
}

void ASpawner::PreventPlayersFromGoingBackwards()
{
	current_y_progress = StaticFunctionLibrary::FindLeftMostPlayerProgress(GetWorld()) + left_deviation < current_y_progress ? StaticFunctionLibrary::FindLeftMostPlayerProgress(GetWorld()) + left_deviation : current_y_progress;
	StaticFunctionLibrary::ConstrainCharactersToAxisProgress(GetWorld(), FVector(-1, current_y_progress, -1));
}

void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RespwanAfterFall();
	// PreventPlayersFromGoingBackwards();
}

AUberbotsCharacter* ASpawner::SpawnPlayer(int spawner_id)
{
	AUberbotsCharacter* player = nullptr;
    APlayerController* PC = UGameplayStatics::CreatePlayer(this, spawner_id, true);
    if (PC != nullptr)
    {
        player = Cast<AUberbotsCharacter>(PC->GetCharacter());
    }
	
    if (player == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("player cast failed spawner cpp line 38"));
	}

	if (player != nullptr)
		PlayerSpawn.Broadcast(player);
	else
		UE_LOG(LogTemp, Warning, TEXT("player pointer null"));
	return player;
}

void ASpawner::SpawnPlayers(int amount)
{
	amount = FMath::Clamp(amount, min_players, max_players);
	for (int i = 0; i < amount; ++i)
	{
		FVector average_position = StaticFunctionLibrary::GetAveragePlayerPosition(GetWorld());
		AUberbotsCharacter* player = SpawnPlayer(-1);
		RepositionFromTo(player, FVector (average_position.X, average_position.Y, default_spawn_height));
	}
}

void ASpawner::WhenPlayerSpawns(AUberbotsCharacter * spawned_character)
{
	spawned_character->ConstrainPhysicsTo2DPlane();
	// spawned_character->IgnoreCollisionsWithOtherCharacters(); depricated
	spawned_character->SetUpCollisionLogic();
	RepositionFromTo(spawned_character, StaticFunctionLibrary::GetAveragePlayerPositionExceptFor(GetWorld(), spawned_character));
}
