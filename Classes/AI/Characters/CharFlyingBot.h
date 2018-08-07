#pragma once

#include "CoreMinimal.h"
#include "AI/Characters/CharBot.h"
#include "Navigation/Waypoint.h"
#include "CharFlyingBot.generated.h"

UENUM(BlueprintType)
enum class EPatternEnum : uint8
{
	PE_NONE 	UMETA(DisplayName = "None"),
	PE_SINE_V 	UMETA(DisplayName = "Sine Vertical"),
	PE_SINE_H 	UMETA(DisplayName = "Sine Horizontal"),
	PE_CIRCULAR	UMETA(DisplayName = "Circular"),
	PE_PATROL   UMETA(DisplayName = "Patrol")
};

UCLASS()
class UBERBOTS_API ACharFlyingBot : public ACharBot
{
	GENERATED_BODY()

public:

	ACharFlyingBot();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") EPatternEnum flying_pattern_type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (UIMin = "0.0", UIMax = "1.0")) float follower_speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Screen") bool lock_with_screen;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Screen", meta = (UIMin = "-500.0", UIMax = "500.0")) float v_screen_deviation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Screen", meta = (UIMin = "-500.0", UIMax = "500.0")) float h_screen_deviation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Oscillation", meta = (UIMin = "0.0", UIMax = "500.0")) float oscillation_amplitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Oscillation", meta = (UIMin = "0.0", UIMax = "0.2")) float oscillation_speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Circular", meta = (UIMin = "0.0", UIMax = "500.0")) float radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Circular", meta = (UIMin = "0.0", UIMax = "0.2")) float circle_speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Patrol") bool dart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Patrol", meta = (UIMin = "0.0", UIMax = "0.3")) float patrol_speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Patrol", meta = (UIMin = "0.0", UIMax = "100.0")) float crit_dist_to_patrol_target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Patrol") TArray<AWaypoint*> waypoints;


	float oscillation_seed;
	float circle_seed;
	int patrol_progress;
	FVector last_frame_position;

protected:

	virtual void PossessedBy(AController * possessor) override;

};