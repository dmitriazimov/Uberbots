// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "UberbotsCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Inventory/WeaponComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EngineUtils.h"
#include "AI/Characters/CharBot.h"

AUberbotsCharacter::AUberbotsCharacter()
{
    PrimaryActorTick.TickGroup = TG_PrePhysics;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	max_health = 100.0f;
	health = max_health;
	range = 0.0f;
	ranged_damage = 0.0f;
	melee_damage = 0.0f;
	melee_hit_knock_back_force = 0.0f;

	SetUpCollisionLogic();
}

void AUberbotsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bIsFiring)
    {
        UWeaponComponent* Weapon = Cast<UWeaponComponent>(GetComponentByClass(UWeaponComponent::StaticClass()));
        if (Weapon != nullptr)
            Weapon->Fire();
    }
}

float AUberbotsCharacter::GetAimValue()
{
    return AimValue;
}

void AUberbotsCharacter::ConstrainPhysicsTo2DPlane()
{
	for (TActorIterator<AUberbotsCharacter> living_characters_iter(GetWorld()); living_characters_iter; ++living_characters_iter)
	{
		(*living_characters_iter)->SetActorLocation(FVector(side_scroll_X, (*living_characters_iter)->GetActorLocation().Y, (*living_characters_iter)->GetActorLocation().Z));
		(*living_characters_iter)->GetCharacterMovement()->SetPlaneConstraintFromVectors(FVector(0, 1, 0), FVector(0, 0, 1));
		(*living_characters_iter)->GetCharacterMovement()->bConstrainToPlane = true;
	}
}

void AUberbotsCharacter::SetUpCollisionLogic()
{
	TArray<UPrimitiveComponent*> capsules;
	GetComponents<UPrimitiveComponent>(capsules);
	for (auto capsule : capsules)
	{
		capsule->bGenerateOverlapEvents = true;
		capsule->SetNotifyRigidBodyCollision(true);
		capsule->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4); // letting it know it's a player
		capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECollisionResponse::ECR_Ignore); // players
		capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel5, ECollisionResponse::ECR_Overlap); // AI
	}
}

float AUberbotsCharacter::GetDamaged(float damage)
{
	health -= damage;
	if (health < 0.0f)
	{
		Death();
	}
	return health;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AUberbotsCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AUberbotsCharacter::BeginFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &AUberbotsCharacter::EndFire);
	
    PlayerInputComponent->BindAxis("MoveRight", this, &AUberbotsCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Aim", this, &AUberbotsCharacter::Aim);
    PlayerInputComponent->BindAxis("AimMouse", this, &AUberbotsCharacter::AimMouse);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &AUberbotsCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AUberbotsCharacter::TouchStopped);
}

void AUberbotsCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AUberbotsCharacter::Death()
{
	// TODO figure out if respawn or straight out death
}

void AUberbotsCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f,-1.f,0.f), Value);
}

void AUberbotsCharacter::Aim(float Val)
{
    // Valid only when controlled by gamepad
    if (IsGamepadControlled())
    {
        AimValue = Val;
    }
}

void AUberbotsCharacter::AimMouse(float Val)
{
    // Valid only when controlled by mouse
    if (!IsGamepadControlled())
    {
        MouseAim += Val * 0.1f; // Todo : make the scale a setting
        MouseAim = FMath::Clamp(MouseAim, -1.f, 1.f);
        AimValue = MouseAim;
    }
}

void AUberbotsCharacter::BeginFire()
{
    bIsFiring = true;
}

void AUberbotsCharacter::EndFire()
{
    bIsFiring = false;
}

void AUberbotsCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void AUberbotsCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

bool AUberbotsCharacter::IsGamepadControlled() const
{
    return Cast<APlayerController>(GetController())->GetLocalPlayer()->GetControllerId() > 0;
}