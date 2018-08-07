// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "Engine.h"
#include "UberbotsCharacter.h"

class UBERBOTS_API StaticFunctionLibrary
{
public:

	static FVector GetAveragePlayerPosition(UWorld * world_context);

	static FVector GetAveragePlayerPositionExceptFor(UWorld * world_context, AUberbotsCharacter* player_character);

	static float FindLeftMostPlayerProgress(UWorld * world_context);

	static void ConstrainCharactersToAxisProgress(UWorld * world_context, FVector progress);

	static AUberbotsCharacter* FindClosestPlayerCharacter(UWorld * world_context, AActor* to);

	static bool InDirectRange(UWorld * world_context, ACharacter* origin, ACharacter* target, float range);

	static int InRelativeRange(UWorld * world_context, ACharacter* origin, ACharacter* target, float range_min, float range_max);

	static void Follow(AActor* follower, FVector destination, float speed);

	static FVector Circle(float radius, float seed, float y, float z);

	StaticFunctionLibrary();
	~StaticFunctionLibrary();
};
