// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKart.generated.h"

USTRUCT()
struct FGokartMove
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float Throttle;
	UPROPERTY()
	float SteeringThrow;

	UPROPERTY()
	float DeltaTime;

	UPROPERTY()
	float Time;
};

USTRUCT()
struct FGokartState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	FGokartMove LastMove;
};

UCLASS()
class MULTIRACING_API AGoKart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGoKart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void SimulateMove(FGokartMove Move);

	FVector GetAirResistance();
	FVector GetRollingResistance();

	void ApplyRotation(float DeltaTime);

	void UpdateLocationFromVelocity(float DeltaTime);

	//자동차의 질량 (kg)
	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;

	UPROPERTY(EditAnywhere)
	float MinTurningRadius = 10;

	//공기저항
	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;

	//바퀴의 구름저항? 0.015는 콘크리트의 저항값
	UPROPERTY(EditAnywhere)
	float RollingRegistanceCoefficient = 0.015;

	void MoveForward(float Value);
	void MoveRight(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(FGokartMove Move);

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FGokartState ServerState;

	FVector Velocity;

	UFUNCTION()
	void OnRep_ServerState();

	UPROPERTY(Replicated)
	float Throttle;
	UPROPERTY(Replicated)
	float SteeringThrow;
};
