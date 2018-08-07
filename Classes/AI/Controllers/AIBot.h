#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UberbotsGameMode.h"
#include "AIBot.generated.h"

UCLASS()
class UBERBOTS_API AAIBot : public AAIController
{
	GENERATED_BODY()

public:

	AAIBot();

	void Clean();

	bool pawn_set = false;
	AUberbotsGameMode* game_mode;

protected:

	virtual void SetPawn(APawn* in_pawn) override;

private:

	void KnockBack(class AUberbotsCharacter * uberbot, class ACharBot* character);
	void ApplyMeleeDamage(class AUberbotsCharacter * uberbot, class ACharBot* character);
};
