// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vehicule.h"
#include "VehiculeIA.generated.h"

UENUM()
enum AlgoUsing
{
	SEEK	  UMETA(DisplayName = "Seek Algo"),
	FLEE	  UMETA(DisplayName = "Flee Algo"),
	ARRIVAL   UMETA(DisplayName = "Arrival Algo"),
	PURSUIT	  UMETA(DisplayName = "Pursuit Algo"),
	EVADE	  UMETA(DisplayName = "Evade Algo"),
};

/**
 * 
 */
UCLASS()
class TP1_IA_API AVehiculeIA : public AVehicule
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVehiculeIA();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Algorithme")
	TEnumAsByte<AlgoUsing> UsingAlgo;

	UPROPERTY(EditAnywhere, Category = "Algorithme")
	float SlowingDistance;

	UPROPERTY(EditAnywhere, Category = "Target")
	AVehicule* Target;

	UPROPERTY(EditAnywhere, Category = "Algorithme")
	float DistanceChange;

	bool IsNew = false;
	FVector NewTargetEvade;
	float Time = 0;
	int Index = 0;

public:
	virtual void Tick(float DeltaTime) override;

	FVector SeekVelocity(FVector TargetLocation);
	FVector FleeVelocity(FVector TargetLocation);
	FVector ArrivalVelocity(FVector TargetLocation);
	FVector PursuitVelocity();
	FVector EvadeVelocity(float DeltaTime);

	void ChangeAlgo();
	AlgoUsing IntToEnum();
};
