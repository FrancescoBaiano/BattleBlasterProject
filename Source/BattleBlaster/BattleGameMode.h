// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Tank.h"

#include "BattleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API ABattleGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	float gameOverDelay = 3.f;

	bool isVictory = false;

	ATank* tank;
	int32 towerCount;

	void ActorDied(AActor* DeadActor);

	void OnGameOverTimerTimeout();
};
