// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Characters/CharBot.h"
#include "Navigation/Waypoint.h"
#include "CharRunningBot.generated.h"

UCLASS()
class UBERBOTS_API ACharRunningBot : public ACharBot
{
	GENERATED_BODY()
	
protected:

	virtual void PossessedBy(AController * possessor) override;

public:

	int patrol_progress;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") TArray<AWaypoint*> waypoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (UIMin = "0.0", UIMax = "500.0")) float crit_dist_to_patrol_target;
};
