#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Map.h"
#include "CameraFocusActor.generated.h"

UCLASS()
class UBERBOTS_API ACameraFocusActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:

	ACameraFocusActor();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FVector right_bound;

private:

	float max_dist = 0.0f;
	const float default_arm_lenght = 500.0f;
	const float interpolation_speed = 0.3f;
	float aspect_ratio;
	float right_bound_left_bump = 0.8f;

	class AUberbotsGameMode* game_mode;

private:

	void FocusOnCharacters(float delta_time);
	void FindMaxDistance();
	void InitViewPoints();
	int GetNumPlayers();
	void UpdateRightBoundPosition();
};
