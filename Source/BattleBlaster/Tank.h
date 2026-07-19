// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"

#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputSubsystems.h"

#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"

#include "Tank.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API ATank : public ABasePawn
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ATank();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* defaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* moveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* turnAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* fireAction;
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* springArmComp;

	UPROPERTY(EditAnywhere)
	float speed = 800.f;

	UPROPERTY(EditAnywhere)
	float turnRate = 100.f;

	APlayerController* playerController;

	bool isAlive = true;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* cameraComp;

	void MoveInput(const FInputActionValue& value);
	void TurnInput(const FInputActionValue& value);

	void HandleDestruction();
	void SetPlayerEnabled(bool enabled);
};
