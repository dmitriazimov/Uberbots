// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Characters/CharBot.h"
#include "Components/PrimitiveComponent.h"
#include "AI/Controllers/AIBot.h"

ACharBot::ACharBot()
{
	PrimaryActorTick.bCanEverTick = true;

	max_health = 100.0f;
	health = max_health;
	range = 0.0f;
	ranged_damage = 0.0f;
	melee_damage = 0.0f;
	melee_hit_knock_back_force = 0.0f;
	active_target = nullptr;
	SetUpCollisionLogic();
}

void ACharBot::OnHit(UPrimitiveComponent * hit_comp, AActor * other_actor, UPrimitiveComponent * other_comp, int a, bool b, const FHitResult & hit)
{
	if (other_actor->IsA(AUberbotsCharacter::StaticClass()) && other_comp != NULL)
	{
		auto uberbot = Cast<AUberbotsCharacter>(other_actor);
		if (uberbot)
		{
			ApplyMeleeDamage(uberbot);
			KnockBack(uberbot);
		}
	}
}

float ACharBot::GetDamaged(float damage)
{
	health -= damage;
	if (health < 0.0f)
	{
		Death();
	}
	return health;
}

void ACharBot::Death()
{
	auto controller = Cast<AAIBot>(GetController());
	if (controller)
	{
		controller->Clean();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("The assigned controller is not of type AAIBot."));
	}
}

void ACharBot::UpdateTarget(AUberbotsCharacter * new_target)
{
	active_target = new_target;
}

void ACharBot::SetUpCollisionLogic()
{
	TArray<UPrimitiveComponent*> capsules;
	GetComponents<UPrimitiveComponent>(capsules);
	for (auto capsule : capsules)
	{
		capsule->bGenerateOverlapEvents = true;
		capsule->SetNotifyRigidBodyCollision(true);
		capsule->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel5); // letting it know it's a player
		capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECollisionResponse::ECR_Overlap); // players
		capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Ignore); // AI
		capsule->OnComponentBeginOverlap.AddDynamic(this, &ACharBot::OnHit);
	}
}

void ACharBot::KnockBack(AUberbotsCharacter * uberbot)
{
	FVector impulse = (uberbot->GetActorLocation() - GetActorLocation()).GetSafeNormal() * melee_hit_knock_back_force;
	Cast<AUberbotsCharacter>(uberbot)->LaunchCharacter(impulse, false, false);
}

void ACharBot::ApplyMeleeDamage(AUberbotsCharacter * uberbot)
{
	uberbot->GetDamaged(melee_damage);
}
