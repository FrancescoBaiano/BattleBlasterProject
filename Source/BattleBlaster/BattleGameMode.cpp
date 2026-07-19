// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Tower.h"

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
}

void ABattleGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == tank)
	{
		tank->HandleDestruction();
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

			}
		}
	}
}
