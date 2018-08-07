// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameModeBase.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "UberbotsGameMode.generated.h"


UCLASS()
class UBERBOTS_API AUberbotsGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AUberbotsGameMode();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;
	void OnControllerConnectionChange(bool Connected, int32 UserID, int32 ControllerID);
	FVector GetCamRightBound();

	FDelegateHandle OnControllerConnectionHandle;
	FVector average_players_location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector round_start_pos = FVector(1207.0f, 109.7f, 230.0f);
	class ACameraFocusActor* main_cam;

protected:
    void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation) override;
};
