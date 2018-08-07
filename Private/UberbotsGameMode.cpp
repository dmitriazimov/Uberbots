// Fill out your copyright notice in the Description page of Project Settings.

#include "UberbotsGameMode.h"
#include "UberbotsCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Logic/Spawner.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Runtime/Core/Public/Misc/CoreDelegates.h"
#include "Logic/StaticFunctionLibrary.h"
#include "Camera/CameraFocusActor.h"

AUberbotsGameMode::AUberbotsGameMode()
{
	average_players_location = FVector::ZeroVector;
	//round_start_pos = FVector(1207.0f, 109.7f, 230.0f);
}

void AUberbotsGameMode::BeginPlay()
{
	FActorSpawnParameters spawn_info;
	spawn_info.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	main_cam = GetWorld()->SpawnActor<ACameraFocusActor>(spawn_info);
	OnControllerConnectionHandle = FCoreDelegates::OnControllerConnectionChange.AddUObject(this, &AUberbotsGameMode::OnControllerConnectionChange);
}

void AUberbotsGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	FCoreDelegates::OnControllerConnectionChange.Remove(OnControllerConnectionHandle);
}

void AUberbotsGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	average_players_location = StaticFunctionLibrary::GetAveragePlayerPosition(GetWorld());
}

void AUberbotsGameMode::OnControllerConnectionChange(bool Connected, int32 UserID, int32 ControllerID)
{
	if (Connected)
	{
		for (TActorIterator<ASpawner> spawner_iter(GetWorld()); spawner_iter; ++spawner_iter)
		{
			(*spawner_iter)->SpawnPlayer(ControllerID + 1);
			return;
		}
	}
	else
	{
		UGameplayStatics::RemovePlayer(UGameplayStatics::GetPlayerController(GetWorld(), ControllerID + 1), true);
	}
}

FVector AUberbotsGameMode::GetCamRightBound()
{
	return main_cam->right_bound;
}

void AUberbotsGameMode::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
    Super::FinishRestartPlayer(NewPlayer, StartRotation);

    AUberbotsCharacter* UberbotsChar = Cast<AUberbotsCharacter>(NewPlayer->GetPawn());
    if (UberbotsChar == nullptr) return;

    UberbotsChar->ConstrainPhysicsTo2DPlane();
}