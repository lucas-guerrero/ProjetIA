// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VehiculeIA.h"
#include "VehiculeCircuit.generated.h"

UENUM()
enum AlgoCuircuit
{
	CIRCUIT	  UMETA(DisplayName = "Circuit"),
	ONE_WAY	  UMETA(DisplayName = "One path"),
	TWO_WAY   UMETA(DisplayName = "Two path"),
};
/**
 * 
 */
UCLASS()
class TP1_IA_API AVehiculeCircuit : public AVehiculeIA
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AVehiculeCircuit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "Algorithme")
	TEnumAsByte<AlgoCuircuit> AlgoCircuit;

	UPROPERTY(EditAnywhere, Category = "Path")
	TArray<AVehicule*> ListTargets;

	UPROPERTY(EditAnywhere, Category = "Path")
	float DistanceChangeTarget;

	UPROPERTY(EditAnywhere, Category = "Path")
	float TimeWait;

	int Direction = 1;
	int IndexList = 0;
	bool IsArrival = false;
	bool IsTarget = false;

	float TimeWaitCurrent = 0;

public:
	virtual void Tick(float DeltaTime) override;

	FVector CalculDirection();
	void ChangeTargetCircuit();
	void ChangeTargetTwo(float DeltaTime);
	void ChangeTargetOne();
};
