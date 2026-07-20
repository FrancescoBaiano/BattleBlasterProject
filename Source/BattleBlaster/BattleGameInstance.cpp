// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleGameInstance.h"

#include "Kismet/GameplayStatics.h"

void UBattleGameInstance::ChangeLevel(int32 index)
{
	if (index > 0 && index <= lastLevelIndex)
	{
		currentLevelIndex = index;

		FString levelNameString = FString::Printf(TEXT("Level_%d"), currentLevelIndex);
		UGameplayStatics::OpenLevel(GetWorld(), *levelNameString);
	}
}

void UBattleGameInstance::LoadNextLevel()
{
	if (currentLevelIndex < lastLevelIndex)
		ChangeLevel(currentLevelIndex + 1);
	else
		RestartGame();
}

void UBattleGameInstance::RestartCurrentLevel()
{
	ChangeLevel(currentLevelIndex);
}

void UBattleGameInstance::RestartGame()
{
	ChangeLevel(1);
}