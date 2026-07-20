// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Tower.h"
#include "BattleGameInstance.h"

void ABattleGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> towers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), towers);
	towerCount = towers.Num();
	UE_LOG(LogTemp, Display, TEXT("Number of towers: %d"), towerCount);

	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (playerPawn) 
	{
		tank = Cast<ATank>(playerPawn);
		if (!tank)
		{
			UE_LOG(LogTemp, Error, TEXT("GameMode Failed To Find Tank Actor"));
		}
	}

	int32 i = 0;
	while (i < towerCount)
	{
		AActor* towerActor = towers[i];
		if (towerActor)
		{
			ATower* tower = Cast<ATower>(towerActor);
			if (tower && tank)
			{
				tower->tank = tank;
			}
		}
		i++;
	}

	countdownSeconds = countdownDelay;
	GetWorldTimerManager().SetTimer(countdownTimerHandle, this, &ABattleGameMode::OnCountdownTimerTimeout, 1.f, true);
}

void ABattleGameMode::OnCountdownTimerTimeout()
{
	countdownSeconds --;
	if (countdownSeconds > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Countdown: %d"), countdownSeconds);
	}
	else if (countdownSeconds == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("GAME START!"));
		tank->SetPlayerEnabled(true);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(countdownTimerHandle);
	}
}

void ABattleGameMode::ActorDied(AActor* DeadActor)
{
	bool isGameOver = false;

	if (DeadActor == tank)
	{
		tank->HandleDestruction();
		isGameOver = true;
	}
	else
	{
		ATower* deadTower = Cast<ATower>(DeadActor);
		if (deadTower)
		{
			deadTower->HandleDestruction();
			towerCount--;
			if (towerCount == 0)
			{
				isGameOver = true;
				isVictory = true;
			}
		}
	}

	if (isGameOver)
	{
		FString gameOverString = isVictory ? "Victory" : "Defeat";

		UE_LOG(LogTemp, Warning, TEXT("Game is Over: %s"), *gameOverString);

		FTimerHandle gameOverTimerHandle;
		GetWorldTimerManager().SetTimer(gameOverTimerHandle, this, &ABattleGameMode::OnGameOverTimerTimeout, gameOverDelay, false);
	}
}

void ABattleGameMode::OnGameOverTimerTimeout()
{
	UGameInstance* gameInstance = GetGameInstance();

	if (gameInstance)
	{
		UBattleGameInstance* battleGameInstance = Cast<UBattleGameInstance>(gameInstance);
		if (battleGameInstance)
		{
			if (isVictory)
			{
				battleGameInstance->LoadNextLevel();
			}
			else
			{
				battleGameInstance->RestartCurrentLevel();
			}
		}
	}
}