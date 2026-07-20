// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank()
{
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(capsuleComp);

	cameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	cameraComp->SetupAttachment(springArmComp);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	playerController = Cast<APlayerController>(Controller);
	if (playerController)
	{
		if (ULocalPlayer* localPlayer = playerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(localPlayer))
			{
				subsystem->AddMappingContext(defaultMappingContext, 0);
			}
		}
	}

	SetPlayerEnabled(false);
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (playerController)
	{
		FHitResult hitResult;
		playerController->GetHitResultUnderCursor(ECC_Visibility, false, hitResult);
		

		//DrawDebugSphere(GetWorld(), hitResult.ImpactPoint, 25.f, 12, FColor::Red);

		RotateTurret(hitResult.ImpactPoint);
	}
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* eIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (eIC)
	{
		eIC->BindAction(moveAction, ETriggerEvent::Triggered, this, &ATank::MoveInput);
		eIC->BindAction(turnAction, ETriggerEvent::Triggered, this, &ATank::TurnInput);
		eIC->BindAction(fireAction, ETriggerEvent::Started, this, &ATank::Fire);
	}
}

void ATank::MoveInput(const FInputActionValue& value)
{
	float inputValue = value.Get<float>();
	FVector deltaLocation = FVector(inputValue, 0.f, 0.f);
	deltaLocation.X = speed * inputValue * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()); // last thing is deltatime
	AddActorLocalOffset(deltaLocation, true); // true for wall collision
}

void ATank::TurnInput(const FInputActionValue& value)
{
	float inputValue = value.Get<float>();
	FRotator deltaRotation = FRotator(0.f, 0.f, 0.f);
	deltaRotation.Yaw = turnRate * inputValue * GetWorld()->GetDeltaSeconds();
	AddActorLocalRotation(deltaRotation, true);

	//UE_LOG(LogTemp, Warning, TEXT("Turning"));
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	SetPlayerEnabled(false);
	isAlive = false;
}

void ATank::SetPlayerEnabled(bool enabled)
{
	if (playerController)
	{
		if (enabled) 
		{
			EnableInput(playerController);
		}
		else
		{
			DisableInput(playerController);
		}
		playerController->bShowMouseCursor = enabled;
	}
}
