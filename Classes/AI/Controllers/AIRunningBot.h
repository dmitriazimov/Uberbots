#pragma once

#include "CoreMinimal.h"
#include "AI/Controllers/AIBot.h"
#include "AIRunningBot.generated.h"

UCLASS()
class UBERBOTS_API AAIRunningBot : public AAIBot
{
	GENERATED_BODY()
	
public:

	virtual void Tick(float DeltaTime) override;
	void InitFalling(class ACharRunningBot* character);

	UFUNCTION() void OnMovementModeChanged(ACharacter* MovedCharacter, EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0);
	UFUNCTION() void OnLanded(const FHitResult& Hit);

protected:

	virtual void SetPawn(APawn* in_pawn) override;

private:

	void Patrol();

	class ACharRunningBot* running_char;
	bool moving;
};
