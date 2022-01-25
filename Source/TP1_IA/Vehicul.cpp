// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicul.h"
#include "Target.h"

#include <Kismet/GameplayStatics.h>

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

	if (SteeringAlgo == SteeringAlgo::PURSUIT)
	{
		TArray<AActor*> Vehiculs;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVehicul::StaticClass(), Vehiculs);

		int32 Index = FMath::RandRange(0, Vehiculs.Num());
		PursuitActor = Vehiculs[Index];
	}
	else
	{
		TArray<AActor*> Targets;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATarget::StaticClass(), Targets);

		int32 Index = FMath::RandRange(0, Targets.Num()-1);
		TargetActor = Targets[Index];
	}
}

// Called every frame
void AVehicul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector SteeringDirection;
	/*
	if(SteeringAlgo == SteeringAlgo::SEEK) SteeringDirection = SeekVelocity(FVector(100, 0, GetActorLocation().Z));
	else SteeringDirection = FleeVelocity(FVector(100, 0, GetActorLocation().Z));
	*/

	if (SteeringAlgo == SteeringAlgo::SEEK) SteeringDirection = SeekVelocity(TargetActor->GetActorLocation());
	else SteeringDirection = FleeVelocity(TargetActor->GetActorLocation());

	FVector SteeringForce = Truncate(SteeringDirection, MaxForce);
	FVector Acceleration = SteeringForce / Mass;
	Velocity = Truncate(Velocity + Acceleration, MaxSpeed);
	SetActorLocation(GetActorLocation() + Velocity);

	/*
	FVector VNorm = FVector(Velocity.X, Velocity.Y, Velocity.Z);
	VNorm.Normalize();
	FRotator Rotation = FRotator(VNorm.X, VNorm.Y, VNorm.Z);
	SetActorRotation(Rotation);
	*/
}

FVector AVehicul::SeekVelocity(FVector Target)
{
	FVector VectorDist = Target - GetActorLocation();
	VectorDist.Normalize();
	FVector VelocityDesired = VectorDist * MaxSpeed;
	return VelocityDesired - Velocity;
}

FVector AVehicul::FleeVelocity(FVector Target)
{
	FVector VectorDist = Target - GetActorLocation();
	VectorDist = -VectorDist;
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

