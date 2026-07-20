// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Tank.h"
#include "ScreenMessage.h"

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
	TSubclassOf<UScreenMessage> screenMessageClass;

	UScreenMessage* screenMessageWidget;

	UPROPERTY(EditAnywhere)
	float gameOverDelay = 3.f;

	UPROPERTY(EditAnywhere)
	int32 countdownDelay = 3;

	int32 countdownSeconds;

	FTimerHandle countdownTimerHandle;

	bool isVictory = false;

	ATank* tank;
	int32 towerCount;

	void ActorDied(AActor* DeadActor);

	void OnGameOverTimerTimeout();

	void OnCountdownTimerTimeout();
};
