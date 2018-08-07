#pragma once

#include "CoreMinimal.h"
#include "AI/Controllers/AIBot.h"
#include "AIFlyingBot.generated.h"

UCLASS()
class UBERBOTS_API AAIFlyingBot : public AAIBot
{
	GENERATED_BODY()

public:

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void SetPawn(APawn* InPawn) override;

private:

	void OscillateVertically();
	void OscillateHorizintally();
	void InitAnchor();
	void Circle();
	void Patrol();
	void Move();
	bool OverlappedWithWaypoint();

	class ACharFlyingBot* flying_char;
	FVector target_position;
};
