// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateLevels.h"
#include "Wall.h"
#include "Whole.h"
#include "Vehicule.h"
#include "Ground.h"

#include <string>
#include <fstream>
#include <math.h>

// Sets default values
AGenerateLevels::AGenerateLevels()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	UnitBlock = 100.f;
}

// Called when the game starts or when spawned
void AGenerateLevels::BeginPlay()
{
	Super::BeginPlay();

	std::string NameFile = TCHAR_TO_UTF8(*FPaths::ProjectDir());
	NameFile = NameFile + "/Content/PathFinding.txt";

	std::ifstream File(NameFile);
	if (File.is_open())
	{
		std::string Line;

		File >> Line >> SizeMap;

		int x = 0;
		int y = 0;

		while (std::getline(File, Line))
		{
			TArray<struct Tile> Row;
			y = 0;
			for (char c : Line)
			{
				struct Tile Tile;
				Tile.Letter = c;
				Tile.IsTraitment = false;
				Tile.IsWhole = false;
				Tile.IsWalked = true;
				Tile.Cost = 1;
				Tile.CostActual = 0;
				switch (c)
				{
				case 'X':
					Tile.IsWalked = false;
					GenerateWall(x, y);
					break;
				case '0':
					GeneratePlayer(x, y);
					GenerateGround(x, y);
					break;
				case '1':
					Tile.IsWhole = true;
					GenerateWhole(x, y);
					break;
				default:
					GenerateGround(x, y);
					break;
				}
				Row.Add(Tile);
				++y;
			}
			++x;
			Map.Add(Row);
		}
	}
	File.close();
}

AActor* AGenerateLevels::GenerateWall(int x, int y)
{
	AActor* Actor = nullptr;
	if (WallClass)
	{
		float Decalage = SizeMap / 2 * UnitBlock;

		FVector SpawnLocation = FVector(-x * UnitBlock + Decalage, y * UnitBlock - Decalage, 0.f);
		FTransform SpawnTransform(GetActorRotation(), SpawnLocation);
		Actor = GetWorld()->SpawnActor<AWall>(WallClass, SpawnTransform);
	}
	return Actor;
}

AActor* AGenerateLevels::GeneratePlayer(int x, int y)
{
	AActor* Actor = nullptr;
	if (VehiculeClass)
	{
		float Decalage = SizeMap / 2 * UnitBlock;

		FVector SpawnLocation = FVector(-x * UnitBlock + Decalage, y * UnitBlock - Decalage, 0.f);
		FTransform SpawnTransform(GetActorRotation(), SpawnLocation);
		Actor = GetWorld()->SpawnActor<AVehicule>(VehiculeClass, SpawnTransform);
	}
	return Actor;
}

AActor* AGenerateLevels::GenerateWhole(int x, int y)
{
	AActor* Actor = nullptr;
	if (WholeClass)
	{
		float Decalage = SizeMap / 2 * UnitBlock;

		FVector SpawnLocation = FVector(-x * UnitBlock + Decalage, y * UnitBlock - Decalage, 0.f);
		FTransform SpawnTransform(GetActorRotation(), SpawnLocation);
		Actor = GetWorld()->SpawnActor<AWhole>(WholeClass, SpawnTransform);
	}
	return Actor;
}

AActor* AGenerateLevels::GenerateGround(int x, int y)
{
	AActor* Actor = nullptr;
	if (GroundClass)
	{
		float Decalage = SizeMap / 2 * UnitBlock;

		FVector SpawnLocation = FVector(-x * UnitBlock + Decalage, y * UnitBlock - Decalage, 0.f);
		FTransform SpawnTransform(GetActorRotation(), SpawnLocation);
		Actor = GetWorld()->SpawnActor<AGround>(GroundClass, SpawnTransform);
	}
	return Actor;
}

bool AGenerateLevels::IsValid(int x, int y)
{
	if (x < 1 || x >= SizeMap + 1) return false;
	if (y < 0 || y >= SizeMap) return false;

	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Valid %d : %d"), x, y));

	if (!Map[x][y].IsWalked) return false;
	return true;
}

FIntVector AGenerateLevels::PositionInMap(FVector Location)
{
	float Decalage = SizeMap / 2 * UnitBlock;
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, Location.ToString());

	FVector AdaptLocation;

	AdaptLocation.X = - (Location.X - Decalage) / UnitBlock;
	AdaptLocation.Y = (Location.Y + Decalage) / UnitBlock;

	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, AdaptLocation.ToString());

	return FIntVector(AdaptLocation);
}

FIntVector AGenerateLevels::ClickInPosition(FVector Location)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, Location.ToString());

	FVector AdaptLocation = Location;

	AdaptLocation.X *= -1;
	AdaptLocation.X += SizeMap/2;
	++AdaptLocation.X;

	AdaptLocation.Y += SizeMap/2;

	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, AdaptLocation.ToString());

	return FIntVector(AdaptLocation);
}

FVector AGenerateLevels::GetCoordonne(int x, int y)
{
	float Decalage = SizeMap / 2 * UnitBlock;

	return FVector(-x * UnitBlock + Decalage, y * UnitBlock - Decalage, 0.f);
}

struct Tile &AGenerateLevels::GetTile(int x, int y)
{
	return Map[x][y];
}

void AGenerateLevels::ClearMapAlgo()
{
	for (TArray<struct Tile>& Row : Map)
	{
		for (struct Tile& Tile : Row)
		{
			Tile.CostActual = 0.f;
			Tile.FActual = INFINITY;
			Tile.PreviousPoint = FIntVector(0, 0, 0);
			Tile.IsTraitment = false;
			Tile.IsInList = false;
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Tile Left: CostActual = %f"), Tile.CostActual));
		}
	}
}