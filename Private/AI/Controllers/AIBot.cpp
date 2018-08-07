// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Controllers/AIBot.h"
#include "Logic/StaticFunctionLibrary.h"
#include "AI/Characters/CharBot.h"
#include "UberbotsCharacter.h"
#include "TimerManager.h"

AAIBot::AAIBot()
{
	pawn_set = false;
}

void AAIBot::SetPawn(APawn * in_pawn)
{
	Super::SetPawn(in_pawn);
	game_mode = Cast<AUberbotsGameMode>(GetWorld()->GetAuthGameMode());
	auto character = Cast<ACharBot>(in_pawn);
	if (character)
	{
		character->SetActorLocation(FVector(game_mode->round_start_pos.X, character->GetActorLocation().Y, character->GetActorLocation().Z));
		pawn_set = true; // important to verify before doing anything in event tick
	}
}

void AAIBot::KnockBack(AUberbotsCharacter * uberbot, ACharBot* character)
{
	FVector impulse = (uberbot->GetActorLocation() - character->GetActorLocation()).GetSafeNormal() * character->melee_hit_knock_back_force;
	Cast<AUberbotsCharacter>(uberbot)->LaunchCharacter(impulse, false, false);
}

void AAIBot::ApplyMeleeDamage(AUberbotsCharacter * uberbot, ACharBot* character)
{
	uberbot->GetDamaged(character->melee_damage);
}

void AAIBot::Clean()
{
	GetWorldTimerManager().ClearAllTimersForObject(this);
	UnPossess();
	Destroy();
}