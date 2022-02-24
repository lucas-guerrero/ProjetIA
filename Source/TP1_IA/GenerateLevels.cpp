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
	for (TArray<char> Row : Map)
	{
		for (char c : Row)
		{
			UE_LOG(LogTemp, Warning, TEXT("%c"), c);
		}
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

