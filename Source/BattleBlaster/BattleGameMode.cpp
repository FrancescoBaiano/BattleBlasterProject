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

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (playerController)
	{
		screenMessageWidget = CreateWidget<UScreenMessage>(playerController, screenMessageClass);
		if (screenMessageWidget)
		{
			screenMessageWidget->AddToPlayerScreen();
			screenMessageWidget->SetMessageText("Get Ready!");
		}
	}	

	countdownSeconds = countdownDelay;
	GetWorldTimerManager().SetTimer(countdownTimerHandle, this, &ABattleGameMode::OnCountdownTimerTimeout, 1.f, true);
}

void ABattleGameMode::OnCountdownTimerTimeout()
{
	countdownSeconds --;
	if (countdownSeconds > 0)
	{
		screenMessageWidget->SetMessageText(FString::FromInt(countdownSeconds));
	}
	else if (countdownSeconds == 0)
	{
		screenMessageWidget->SetMessageText("Go!");
		tank->SetPlayerEnabled(true);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(countdownTimerHandle);
		screenMessageWidget->SetVisibility(ESlateVisibility::Hidden);
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

		screenMessageWidget->SetVisibility(ESlateVisibility::Visible);
		screenMessageWidget->SetMessageText(gameOverString);

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