// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UberbotsCharacter.h"
#include "CharBot.generated.h"

UCLASS()
class UBERBOTS_API ACharBot : public ACharacter
{
	GENERATED_BODY()

public:

	ACharBot();

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float max_health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float ranged_damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float melee_damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float melee_hit_knock_back_force;

	UFUNCTION() 
	void OnHit(UPrimitiveComponent * hit_comp, AActor * other_actor, UPrimitiveComponent * other_comp, int a, bool b, const FHitResult & hit);

private:

	AUberbotsCharacter * active_target;

	float GetDamaged(float damage);
	void Death();
	void UpdateTarget(AUberbotsCharacter * new_target);
	void SetUpCollisionLogic();
	void KnockBack(AUberbotsCharacter * uberbot);
	void ApplyMeleeDamage(AUberbotsCharacter * uberbot);
};
