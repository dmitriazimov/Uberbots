#define _USE_MATH_DEFINES

#include "AI/Controllers/AIFlyingBot.h"
#include "AI/Characters/CharFlyingBot.h"
#include "Logic/StaticFunctionLibrary.h"
#include <math.h>
#include "Math/UnrealMathUtility.h"

void AAIFlyingBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (pawn_set)
	{
		Move();
	}
}

void AAIFlyingBot::SetPawn(APawn * in_pawn)
{
	Super::SetPawn(in_pawn);
	flying_char = Cast<ACharFlyingBot>(in_pawn);
	if (!flying_char)
	{
		pawn_set = false;
	}
}

void AAIFlyingBot::OscillateVertically()
{
	InitAnchor();
	target_position.Z += FMath::Sin(flying_char->oscillation_seed) * flying_char->oscillation_amplitude;
	flying_char->oscillation_seed += flying_char->oscillation_speed;
	StaticFunctionLibrary::Follow(flying_char, target_position, flying_char->follower_speed);
}

void AAIFlyingBot::OscillateHorizintally()
{
	InitAnchor();
	target_position.Y += FMath::Sin(flying_char->oscillation_seed) * flying_char->oscillation_amplitude;
	flying_char->oscillation_seed += flying_char->oscillation_speed;
	StaticFunctionLibrary::Follow(flying_char, target_position, flying_char->follower_speed);
}

void AAIFlyingBot::InitAnchor()
{
	if (flying_char->lock_with_screen)
	{
		target_position = game_mode->GetCamRightBound() + FVector(0.0f, -flying_char->h_screen_deviation, flying_char->v_screen_deviation);
	}
	else
	{
		target_position = flying_char->waypoints[0]->GetActorLocation();
	}
}

void AAIFlyingBot::Circle()
{
	if (flying_char->lock_with_screen)
	{
		target_position = StaticFunctionLibrary::Circle(flying_char->radius, flying_char->circle_seed, 
			game_mode->GetCamRightBound().Y - flying_char->h_screen_deviation, 
			game_mode->GetCamRightBound().Z + flying_char->v_screen_deviation);
	}
	else
	{
		target_position = StaticFunctionLibrary::Circle(flying_char->radius, flying_char->circle_seed,
			flying_char->waypoints[0]->GetActorLocation().Y,
			flying_char->waypoints[0]->GetActorLocation().Z);
	}
	target_position.X = game_mode->average_players_location.X;
	flying_char->circle_seed = flying_char->circle_seed + flying_char->circle_speed > 2 * M_PI ? 0.0f : flying_char->circle_seed + flying_char->circle_speed;
	StaticFunctionLibrary::Follow(flying_char, target_position, flying_char->follower_speed);
}

void AAIFlyingBot::Patrol()
{
	if (!flying_char->lock_with_screen)
	{
		if (flying_char->waypoints.Num() > 1)
		{
			if (!flying_char->dart)
			{
				FVector direction = FVector(flying_char->waypoints[flying_char->patrol_progress]->GetActorLocation() - flying_char->GetActorLocation());
				direction /= direction.Size();
				flying_char->SetActorLocation(flying_char->GetActorLocation() + direction * 4.0f);
				if (OverlappedWithWaypoint())
				{
					flying_char->patrol_progress = flying_char->patrol_progress + 1 >= flying_char->waypoints.Num() ? 0 : flying_char->patrol_progress + 1;
				}
			}
			else
			{
				StaticFunctionLibrary::Follow(flying_char, flying_char->waypoints[flying_char->patrol_progress]->GetActorLocation(), flying_char->patrol_speed);
				if ((flying_char->GetActorLocation() - flying_char->waypoints[flying_char->patrol_progress]->GetActorLocation()).Size() < flying_char->crit_dist_to_patrol_target)
				{
					flying_char->patrol_progress = flying_char->patrol_progress + 1 >= flying_char->waypoints.Num() ? 0 : flying_char->patrol_progress + 1;
				}
			}
			flying_char->last_frame_position = flying_char->GetActorLocation();
		}
		else
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Flying AI ordered to patrol but has only one waypoint assigned."));
		}
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Patrolling happens in world space. Set lock_with_screen to false for the flying AI to patrol."));
	}
}

void AAIFlyingBot::Move()
{
	switch (flying_char->flying_pattern_type)
	{
	case EPatternEnum::PE_SINE_V:
		OscillateVertically();
		break;
	case EPatternEnum::PE_SINE_H:
		OscillateHorizintally();
		break;
	case EPatternEnum::PE_CIRCULAR:
		Circle();
		break;
	case EPatternEnum::PE_PATROL:
		Patrol();
		break;
	case EPatternEnum::PE_NONE:
		break;
	}
}

bool AAIFlyingBot::OverlappedWithWaypoint()
{
	float distance_traveled_between_frames = (flying_char->GetActorLocation() - flying_char->last_frame_position).Size();
	float distance_a = (flying_char->waypoints[flying_char->patrol_progress]->GetActorLocation() - flying_char->last_frame_position).Size();
	float distance_b = (flying_char->GetActorLocation() - flying_char->waypoints[flying_char->patrol_progress]->GetActorLocation()).Size();
	return FMath::IsNearlyZero(distance_traveled_between_frames - (distance_a + distance_b), flying_char->crit_dist_to_patrol_target);
}
