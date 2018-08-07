// Fill out your copyright notice in the Description page of Project Settings.

#include "Navigation/Launchpad.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "AI/Navigation/NavigationSystem.h"


ALaunchpad::ALaunchpad(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Launchpad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Launchpad"));
	Launchpad->SetupAttachment(Root);
	Launchpad->SetRelativeLocation(FVector::ZeroVector);

	TriggerVolume = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Volume"));
	TriggerVolume->SetupAttachment(Launchpad);
	TriggerVolume->SetSphereRadius(25.f, true);

	NavLink = CreateDefaultSubobject<UNavLinkComponent>(TEXT("Nav Link"));
	NavLink->SetupAttachment(Root);

	Target = { 0.f, 0.f, 100.f };

	TriggerVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &ALaunchpad::OnTriggerBeginOverlap);
}

FVector ALaunchpad::CalculateLaunchVelocity(AActor * LaunchedActor)
{
	auto Start = LaunchedActor->GetActorLocation();
	Start.Z -= LaunchedActor->GetSimpleCollisionHalfHeight();

	auto End = GetActorTransform().TransformPosition(Target);

	FVector Result;
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, Result, Start, End);
	return Result;
}

void ALaunchpad::OnTriggerBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * Other, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	auto Character = Cast<ACharacter>(Other);

	if (Character)
	{
		LaunchCharacter(Character);
	}
}

void ALaunchpad::LaunchCharacter(ACharacter * Character)
{
	auto MovementComponent = Character->GetCharacterMovement();
	MovementComponent->Velocity = CalculateLaunchVelocity(Character);
	MovementComponent->SetMovementMode(EMovementMode::MOVE_Falling);
}

void ALaunchpad::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);

	UpdateNavLinks();
	Target.X = 0.0f;
}

void ALaunchpad::UpdateNavLinks()
{
	NavLink->SetRelativeLocation(FVector::ZeroVector);

	check(NavLink->Links.Num() == 1);

	auto& Link = NavLink->Links[0];
	Link.Left = FVector::ZeroVector;
	Link.Right = Target;
	Link.Direction = ENavLinkDirection::LeftToRight;

	auto World = GetWorld();
	if (World)
	{
		auto NavSystem = World->GetNavigationSystem();
		if (NavSystem)
		{
			NavSystem->UpdateComponentInNavOctree(*NavLink);
		}
	}
}
