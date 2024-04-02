// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TinyTanksGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATinyTanksGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void ActorDied(AActor* DeadActor);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);

private:
	class ATank* Tank;
	class ATinyTanksPlayerController* TinyTanksPlayerController;

	float StartDelay = 3.f;

	void HandleGameStart();

	int32 TowerCount = 0;
	int32 GetTowerCount();
 };
