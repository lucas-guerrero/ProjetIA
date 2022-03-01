// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenerateLevels.generated.h"

struct Node
{
	int x, y;
};

struct Arc
{
	struct Node *p1, *p2;
};

struct Graph
{
	TArray<struct Node*> ListNodes;
	TArray<struct Arc*> ListArcs;
};

UCLASS()
class TP1_IA_API AGenerateLevels : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGenerateLevels();

	TArray<TArray<char>> Map;
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

	FVector PositionInMap(FVector Location);
	char Get(FVector Location);
	bool IsValid(int x, int y);
};
