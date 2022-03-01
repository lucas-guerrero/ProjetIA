// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateLevels.h"
#include "Wall.h"
#include "Vehicule.h"

#include <string>
#include <fstream>

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

		int x = SizeMap;
		int y = SizeMap;

		while (std::getline(File, Line))
		{
			TArray<char> Row;
			y = 0;
			for (char c : Line)
			{
				Row.Add(c);
				switch (c)
				{
				case 'X':
					GenerateWall(x, y);
					break;
				case '0':
					GeneratePlayer(x, y);
					break;
				default:
					break;
				}
				y++;
			}
			x--;
			Map.Add(Row);
		}
	}
	File.close();

	PrintMap();
}

void AGenerateLevels::PrintMap()
{
	int x = 0;
	int y = 0;
	for (TArray<char> Row : Map)
	{
		y = 0;
		for (char c : Row)
		{
			UE_LOG(LogTemp, Warning, TEXT("%c : %d, %d"), c, x, y);
			++y;
		}
		++x;
	}
}

void AGenerateLevels::GenerateWall(int x, int y)
{
	if (WallClass)
	{
		float Decalage = SizeMap / 2 * UnitBlock;

		FVector SpawnLocation = FVector(x * UnitBlock - Decalage, y * UnitBlock - Decalage, 0.f);
		FTransform SpawnTransform(GetActorRotation(), SpawnLocation);
		GetWorld()->SpawnActor<AWall>(WallClass, SpawnTransform);
	}
}

void AGenerateLevels::GeneratePlayer(int x, int y)
{
	if (VehiculeClass)
	{
		float Decalage = SizeMap / 2 * UnitBlock;

		FVector SpawnLocation = FVector(x * UnitBlock - Decalage, y * UnitBlock - Decalage, 0.f);
		FTransform SpawnTransform(GetActorRotation(), SpawnLocation);
		GetWorld()->SpawnActor<AVehicule>(VehiculeClass, SpawnTransform);
	}
}

bool AGenerateLevels::IsValid(int x, int y)
{
	++x;
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("%d : %d"), x, y));

	if (x < 1 || x >= SizeMap + 1) return false;
	if (y < 0 || y >= SizeMap) return false;

	if (Map[x][y] == 'X') return false;
	return true;
}

FIntVector AGenerateLevels::PositionInMap(FVector Location)
{
	FVector AdaptLocation = Location;

	AdaptLocation.X -= SizeMap/2;
	AdaptLocation.Y += SizeMap/2;

	AdaptLocation.X *= -1;

	return FIntVector(AdaptLocation);
}

FVector AGenerateLevels::GetCoordonne(int x, int y)
{
	float Decalage = SizeMap / 2 * UnitBlock;

	return FVector(x * UnitBlock - Decalage, y * UnitBlock - Decalage, 0.f);
}
