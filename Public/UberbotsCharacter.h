#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UberbotsCharacter.generated.h"

UCLASS(config=Game)
class AUberbotsCharacter : public ACharacter
{
public:
	GENERATED_BODY()

    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable)
    float GetAimValue();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float max_health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float ranged_damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float melee_damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float melee_hit_knock_back_force;

protected:
	/** Called for side to side input */
	void MoveRight(float Val);
    void Aim(float Val);
    void AimMouse(float Val);

    void BeginFire();
    void EndFire();

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	virtual void BeginPlay() override;

	void Death();

public:
	AUberbotsCharacter();

	void ConstrainPhysicsTo2DPlane();
	void SetUpCollisionLogic();
	float GetDamaged(float damage);

protected:
    bool IsGamepadControlled() const;

    bool bIsFiring = false;
    float MouseAim = 0;
    float AimValue = 0;

	const float side_scroll_X = 1207.0f;
};
