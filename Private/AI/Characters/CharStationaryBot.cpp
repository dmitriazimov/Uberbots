// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Characters/CharStationaryBot.h"
#include "AI/Controllers/AIStationaryBot.h"

ACharStationaryBot::ACharStationaryBot()
{
	AIControllerClass = AAIStationaryBot::StaticClass();
}