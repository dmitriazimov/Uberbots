// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Characters/CharRunningBot.h"
#include "Engine.h"
#include "AI/Controllers/AIRunningBot.h"

void ACharRunningBot::PossessedBy(AController * possessor)
{
	Super::PossessedBy(possessor);
	AIControllerClass = AAIRunningBot::StaticClass();
	if (!Cast<AAIRunningBot>(possessor) && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Running character doesn't have running controller."));
	}
	patrol_progress = 0;
	crit_dist_to_patrol_target = 100.0f;
	Cast<UCharacterMovementComponent>(GetMovementComponent())->DefaultLandMovementMode = EMovementMode::MOVE_Walking;
	Cast<UCharacterMovementComponent>(GetMovementComponent())->DefaultWaterMovementMode = EMovementMode::MOVE_Walking;
}