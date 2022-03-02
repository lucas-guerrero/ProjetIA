// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenerateLevels.generated.h"

struct Tile
{
	char Letter;
	bool IsWalked;
	bool IsTraitment;
	bool IsInList;
	float Cost;
	float CostActual;
	FIntVector PreviousPoint;
};

UCLASS()
class TP1_IA_API AGenerateLevels : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGenerateLevels();

	TArray<TArray<struct Tile>> Map;
	int SizeMap;

	UPROPERTY(EditDefaultsOnly, BluePrintReadOnly)
	TSubclassOf<class AWall> WallClass;

	UPROPERTY(EditDefaultsOnly, BluePrintReadOnly)
	TSubclassOf<class AVehicule> VehiculeClass;

	UPROPERTY(EditAnywhere)
	float UnitBlock;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void GenerateWall(int x, int y);
	void GeneratePlayer(int x, int y);

	void PrintMap();

	FIntVector PositionInMap(FVector Location);
	FIntVector ClickInPosition(FVector Location);
	FVector GetCoordonne(int x, int y);
	bool IsValid(int x, int y);

	struct Tile& GetTile(int x, int y);

	void ClearMapAlgo();
};
