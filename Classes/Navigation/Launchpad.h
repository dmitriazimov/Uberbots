// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "AI/Navigation/NavLinkComponent.h"
#include "Launchpad.generated.h"

UCLASS()
class UBERBOTS_API ALaunchpad : public AActor
{
	GENERATED_BODY()
	
public:	
	ALaunchpad(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* Launchpad;

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* TriggerVolume;

	UPROPERTY(VisibleAnywhere)
		class UNavLinkComponent* NavLink;

	UPROPERTY(EditAnywhere)
		UMaterial* mesh_material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MakeEditWidget = ""))
		FVector Target;

	UFUNCTION(BlueprintPure)
		FVector CalculateLaunchVelocity(AActor* LaunchedActor);

	UFUNCTION()
		void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UFUNCTION()
		void LaunchCharacter(ACharacter* Character);

	void OnConstruction(const FTransform& Transform) override;

	void UpdateNavLinks();
	
	
};
