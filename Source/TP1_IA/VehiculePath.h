// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VehiculeIA.h"
#include "VehiculePath.generated.h"

/**
 * 
 */
UCLASS()
class TP1_IA_API AVehiculePath : public AVehiculeIA
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AVehiculePath();

	class AGenerateLevels* Levels;

	UPROPERTY(EditAnywhere, Category = "Path")
	float DistanceChangePoint;

	FIntVector Depart;
	FIntVector Destination;
	TArray<FVector> ListPoint;

	int IndexList;

	bool IsArrival;
	bool NoDestination;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void BindInput();

	void GenerateWay();
	void ChangeTargetOne();

	FVector CastToInt(FIntVector Vector);

public:
	virtual void Tick(float DeltaTime) override;

	void Click();

private:
	float f(FIntVector Point, float CostCourant);
	float Distance(FIntVector Point);

	bool CompareFIntVector(const FIntVector& A, const FIntVector& B);

	void AddWayInParcour();
};
