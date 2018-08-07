// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera/CameraFocusActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UberbotsGameMode.h"
#include "EngineUtils.h"
#include "Logic/StaticFunctionLibrary.h"
#include "Engine.h"

// Sets default values
ACameraFocusActor::ACameraFocusActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = default_arm_lenght;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 75.f);
	CameraBoom->RelativeRotation = FRotator(0.f, 180.f, 0.f);

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Perspective;
}

// Called when the game starts or when spawned
void ACameraFocusActor::BeginPlay()
{
	Super::BeginPlay();
	InitViewPoints();
	game_mode = Cast<AUberbotsGameMode>(GetWorld()->GetAuthGameMode());
	UpdateRightBoundPosition();
}

// Called every frame
void ACameraFocusActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FindMaxDistance();
	FocusOnCharacters(DeltaTime);
	UpdateRightBoundPosition();
}

void ACameraFocusActor::FocusOnCharacters(float delta_time)
{
	if (game_mode)
	{
		StaticFunctionLibrary::Follow(this, game_mode->average_players_location, interpolation_speed);
		CameraBoom->TargetArmLength = FMath::Lerp(CameraBoom->TargetArmLength, max_dist * 1.3f, interpolation_speed);
	}
}

void ACameraFocusActor::FindMaxDistance()
{
	max_dist = default_arm_lenght;
	if (GetNumPlayers() > 1)
	{
		for (TActorIterator<APlayerController> player_iter(GetWorld()); player_iter; ++player_iter)
		{
			for (TActorIterator<APlayerController> player_iter_other(GetWorld()); player_iter_other; ++player_iter_other)
			{
				float dist = FMath::Max(FMath::Abs(player_iter_other->GetCharacter()->GetActorLocation().Y - player_iter->GetCharacter()->GetActorLocation().Y),
					FMath::Abs(player_iter_other->GetCharacter()->GetActorLocation().Z - player_iter->GetCharacter()->GetActorLocation().Z) * SideViewCameraComponent->AspectRatio);
				max_dist = dist > max_dist + max_dist * 0.1 ? dist : max_dist;
			}
		}
	}
}

void ACameraFocusActor::InitViewPoints()
{
	GetWorld()->GetGameViewport()->SetDisableSplitscreenOverride(true);
	UGameplayStatics::GetPlayerController(this, 0)->SetViewTarget(this);
}

int ACameraFocusActor::GetNumPlayers()
{
	int num = 0;
	for (TActorIterator<APlayerController> player_iter(GetWorld()); player_iter; ++player_iter)
	{
		++num;
	}
	return num;
}

void ACameraFocusActor::UpdateRightBoundPosition()
{
	if (game_mode && SideViewCameraComponent->FieldOfView != 180.0)
	{
		float y_deviation = CameraBoom->TargetArmLength * (FMath::Tan(SideViewCameraComponent->FieldOfView/2.0f) - right_bound_left_bump);
		right_bound = FVector(game_mode->average_players_location.X, game_mode->average_players_location.Y - y_deviation, GetActorLocation().Z);
	}
}