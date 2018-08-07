#define _USE_MATH_DEFINES

#include "AI/Characters/CharFlyingBot.h"
#include "AI/Controllers/AIFlyingBot.h"
#include "Engine.h"
#include <math.h>

ACharFlyingBot::ACharFlyingBot()
{
	flying_pattern_type = EPatternEnum::PE_NONE;
	lock_with_screen = false;
	oscillation_amplitude = 0.0f;
	oscillation_speed = 0.0f;
	follower_speed = 0.5f;
	v_screen_deviation = 0.0f;
	h_screen_deviation = 0.0f;
	radius = 0.0f;
	circle_speed = 0.0f;
	circle_seed = FMath::RandRange(0.0f, 2 * M_PI);
	oscillation_seed = FMath::RandRange(0.0f, 2 * M_PI);
	patrol_progress = 0;
	dart = false;
	crit_dist_to_patrol_target = 0.1f;
	last_frame_position = FVector::ZeroVector;
	AIControllerClass = AAIFlyingBot::StaticClass();
	Cast<UCharacterMovementComponent>(GetMovementComponent())->DefaultLandMovementMode = EMovementMode::MOVE_Flying;
	Cast<UCharacterMovementComponent>(GetMovementComponent())->DefaultWaterMovementMode = EMovementMode::MOVE_Flying;
}

void ACharFlyingBot::PossessedBy(AController * possessor)
{
	Super::PossessedBy(possessor);
	if (!Cast<AAIFlyingBot>(possessor) && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Flying character doesn't have flying controller."));
	}
	if (!lock_with_screen)
	{
		if (waypoints.Num() == 0)
		{
			FActorSpawnParameters spawn_info;
			spawn_info.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AWaypoint* waypoint = GetWorld()->SpawnActor<AWaypoint>(GetActorLocation(), GetActorRotation(), spawn_info);
			if (waypoint)
				waypoints.Add(waypoint);
		}
	}
}