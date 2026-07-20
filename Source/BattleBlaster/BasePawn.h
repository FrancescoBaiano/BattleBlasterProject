// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/CapsuleComponent.h"
#include "Projectile.h"
#include "NiagaraFunctionLibrary.h"

#include "BasePawn.generated.h"

UCLASS()
class BATTLEBLASTER_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* capsuleComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* baseMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* turretMesh;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* projectileSpawnPoint;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> projectileClass;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* deathParticles;

	void RotateTurret(FVector lookAtTarget);

	void Fire();

	void HandleDestruction();
};
