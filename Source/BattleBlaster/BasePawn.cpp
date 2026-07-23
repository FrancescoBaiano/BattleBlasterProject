// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(capsuleComp);

	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	baseMesh->SetupAttachment(capsuleComp);

	turretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	turretMesh->SetupAttachment(baseMesh);

	projectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("projectileSpawnPoint"));
	projectileSpawnPoint->SetupAttachment(turretMesh);
}

void ABasePawn::RotateTurret(FVector lookAtTarget)
{
	FVector vectorToTarget = lookAtTarget - turretMesh->GetComponentLocation();
	
	FRotator lookAtRotation = FRotator(0.f, vectorToTarget.Rotation().Yaw, 0.f);

	FRotator interpolatedRotation = FMath::RInterpTo(turretMesh->GetComponentRotation(), lookAtRotation, GetWorld()->GetDeltaSeconds(), 10.f);
	
	turretMesh->SetWorldRotation(interpolatedRotation);
}

void ABasePawn::Fire()
{
	FVector spawnLocation = projectileSpawnPoint->GetComponentLocation();
	FRotator spawnRotation = projectileSpawnPoint->GetComponentRotation();

	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(projectileClass, spawnLocation, spawnRotation);
	if (projectile) 
	{
		projectile->SetOwner(this);
	}
	//DrawDebugSphere(GetWorld(), spawnLocation, 20.f, 8, FColor::Red, false, 3.f);
}

void ABasePawn::HandleDestruction()
{
	if (deathParticles)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), deathParticles, GetActorLocation(), GetActorRotation());
	}
	if (deathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), deathSound, GetActorLocation());
	}
	if (deathCameraShakeClass)
	{
		APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (playerController)
		{
			playerController->ClientStartCameraShake(deathCameraShakeClass);
		}
	}
}


