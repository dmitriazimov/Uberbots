#include "AI/Controllers/AIRunningBot.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/Characters/CharRunningBot.h"
#include "Engine.h"

void AAIRunningBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (pawn_set)
	{
		Patrol();
	}
}

void AAIRunningBot::OnMovementModeChanged(ACharacter * MovedCharacter, EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	if (MovedCharacter->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling)
	{
		GetPathFollowingComponent()->Deactivate();
	}
}

void AAIRunningBot::OnLanded(const FHitResult & Hit)
{
	GetPathFollowingComponent()->Activate();
}

void AAIRunningBot::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	running_char = Cast<ACharRunningBot>(InPawn);
	if (running_char)
	{
		InitFalling(running_char);
		moving = false;
	}
}

void AAIRunningBot::Patrol()
{
	if (running_char->waypoints.Num() > 0)
	{
		if (!moving)
		{
			MoveToLocation(running_char->waypoints[running_char->patrol_progress]->GetActorLocation(), -1.0f, false, true, false, true, 0, true);
			moving = true;
			return;
		}
		if ((running_char->GetActorLocation() - running_char->waypoints[running_char->patrol_progress]->GetActorLocation()).Size() < running_char->crit_dist_to_patrol_target)
		{
			moving = false;
			running_char->patrol_progress = running_char->patrol_progress + 1 >= running_char->waypoints.Num() ? 0 : running_char->patrol_progress + 1;
		}
	}
}

void AAIRunningBot::InitFalling(ACharRunningBot* character)
{
	character->MovementModeChangedDelegate.AddUniqueDynamic(this, &AAIRunningBot::OnMovementModeChanged);
	character->LandedDelegate.AddUniqueDynamic(this, &AAIRunningBot::OnLanded);
}