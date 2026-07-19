// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle FireTimerHandle;
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATower::CheckFireCondition, fireRate, true);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsInFireRange())
	{
		RotateTurret(tank->GetActorLocation());
	}	
}

void ATower::CheckFireCondition()
{
	if (tank && tank->isAlive && IsInFireRange())
	{
		Fire();
	}
}

bool ATower::IsInFireRange()
{
	bool result = false;
	if (tank)
	{
		float distanceToTank = FVector::Dist(GetActorLocation(), tank->GetActorLocation());
		result = (distanceToTank <= fireRange);
	}
	return result;
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();

	Destroy();
}
