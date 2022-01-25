// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Vehicul.generated.h"

UENUM()
enum SteeringAlgo
{
	SEEK     UMETA(DisplayName = "Seek Algo"),
	FLEE      UMETA(DisplayName = "Flee Algo"),
	PURSUIT   UMETA(DisplayName = "Pursuit Algo"),
};

UCLASS()
class TP1_IA_API AVehicul : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVehicul();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<SteeringAlgo> SteeringAlgo;

	UPROPERTY(EditAnywhere)
	float Mass = 10.f;

	UPROPERTY(EditAnywhere)
	float MaxForce = 1.f;

	UPROPERTY(EditAnywhere)
	float MaxSpeed = 10.f;

	UPROPERTY(EditAnywhere)
	FVector Velocity;

	AActor* TargetActor;
	AActor* PursuitActor;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	FVector Truncate(FVector Vector, float Max);
	FVector SeekVelocity(FVector Target);
	FVector FleeVelocity(FVector Target);
};
