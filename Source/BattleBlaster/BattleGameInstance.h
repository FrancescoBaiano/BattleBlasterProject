// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BattleGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API UBattleGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	int32 lastLevelIndex = 3;

	UPROPERTY(VisibleAnywhere)
	int32 currentLevelIndex = 1;

	void LoadNextLevel();
	void RestartCurrentLevel();
	void RestartGame();

private:
	void ChangeLevel(int32 index);
};
