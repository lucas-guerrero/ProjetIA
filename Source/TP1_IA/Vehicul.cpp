// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicul.h"

// Sets default values
AVehicul::AVehicul()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVehicul::BeginPlay()
{
	Super::BeginPlay();

	Velocity = FVector(-100, -100, 0);
	Mass = 100.f;
	MaxForce = 10.f;
	MaxSpeed = 5.f;
}

// Called every frame
void AVehicul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector SteeringDirection = SeekVelocity(FVector(100, 0, GetActorLocation().Z));
	FVector SteeringForce = Truncate(SteeringDirection, MaxForce);
	FVector Acceleration = SteeringForce / Mass;
	Velocity = Truncate(Velocity + Acceleration, MaxSpeed);
	SetActorLocation(GetActorLocation() + Velocity);
}

FVector AVehicul::SeekVelocity(FVector Target)
{
	FVector VectorDist = Target - GetActorLocation();
	VectorDist.Normalize();
	FVector VelocityDesired = VectorDist * MaxSpeed;
	return VelocityDesired - Velocity;
}

FVector AVehicul::Truncate(FVector Vector, float Max)
{
	if (Vector.Size() > Max)
	{
		Vector.Normalize();
		return  Vector * Max;
	}
	else return Vector;
}

