// Fill out your copyright notice in the Description page of Project Settings.


#include "TinyTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "TinyTanksPlayerController.h"
#include "TimerManager.h"

void ATinyTanksGameMode::ActorDied(AActor* DeadActor)
{
    if (DeadActor == Tank)
    {
        Tank->HandleDestruction();

        if(TinyTanksPlayerController)
        {
            TinyTanksPlayerController->SetPlayerEnabledState(false);
        }
        GameOver(false);
    }
    else if (ATower* DeadTower = Cast<ATower>(DeadActor))
    {
        DeadTower->HandleDestruction();
        --TowerCount;
        if(TowerCount == 0)
        {
            GameOver(true);
        }
    }
}

void ATinyTanksGameMode::BeginPlay()
{
    Super::BeginPlay();

    HandleGameStart();
}

void ATinyTanksGameMode::HandleGameStart()
{
    TowerCount = GetTowerCount();
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    TinyTanksPlayerController = Cast<ATinyTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    StartGame();

    if(TinyTanksPlayerController)
    {
        TinyTanksPlayerController->SetPlayerEnabledState(false);

        FTimerHandle PlayerEnableTimerHandle;
        FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
            TinyTanksPlayerController,
            &ATinyTanksPlayerController::SetPlayerEnabledState,
            true);

        GetWorldTimerManager().SetTimer(
            PlayerEnableTimerHandle, 
            PlayerEnableTimerDelegate,
            StartDelay,
            false);

    }  
    
}

int32 ATinyTanksGameMode::GetTowerCount()
{
    TArray<AActor*> TowersInGame;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), TowersInGame);
    return TowersInGame.Num();
}