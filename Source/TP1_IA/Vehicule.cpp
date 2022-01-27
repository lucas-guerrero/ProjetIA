// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicule.h"

// Sets default values
AVehicule::AVehicule()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AVehicule::BeginPlay()
{
	Super::BeginPlay();
}

void AVehicule::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector AVehicule::Truncate(FVector Vector, float Max)
{
	if (Vector.Size() > Max)
	{
		Vector.Normalize();
		return  Vector * Max;
	}
	else return Vector;
}