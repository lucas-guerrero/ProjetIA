// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiculeIA.h"


#include <Kismet/KismetMathLibrary.h>

AVehiculeIA::AVehiculeIA()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AVehiculeIA::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AVehiculeIA::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector TargetLocation = Target->GetActorLocation();

	FVector SteeringDirection;

	if (UsingAlgo == AlgoUsing::SEEK) SteeringDirection = SeekVelocity(TargetLocation);
	else if (UsingAlgo == AlgoUsing::FLEE) SteeringDirection = FleeVelocity(TargetLocation);
	else if(UsingAlgo == AlgoUsing::ARRIVAL) SteeringDirection = ArrivalVelocity(TargetLocation);
	else if (UsingAlgo == AlgoUsing::PURSUIT) SteeringDirection = PursuitVelocity();
	else SteeringDirection = EvadeVelocity(DeltaTime);

	FVector SteeringForce = Truncate(SteeringDirection, MaxForce);
	FVector Acceleration = SteeringForce / Mass;
	Velocity = Truncate(Velocity + Acceleration, MaxSpeed);
	SetActorLocation(GetActorLocation() + Velocity);

	SetActorRotation(FRotator(Velocity.Rotation()));
}

FVector AVehiculeIA::SeekVelocity(FVector TargetLocation)
{
	FVector VectorDist = TargetLocation - GetActorLocation();
	VectorDist.Normalize();
	FVector VelocityDesired = VectorDist * MaxSpeed;
	return VelocityDesired - Velocity;
}

FVector AVehiculeIA::ArrivalVelocity(FVector TargetLocation)
{
	FVector VectorDist = TargetLocation - GetActorLocation();
	float Distance = VectorDist.Size();

	float RampedSpeed = MaxSpeed * (Distance / SlowingDistance);

	float ClippedSpeed = FMath::Min(RampedSpeed, MaxSpeed);

	FVector VelocityDesired = VectorDist * (ClippedSpeed / Distance);
	return VelocityDesired - Velocity;
}

FVector AVehiculeIA::FleeVelocity(FVector TargetLocation)
{
	FVector VectorDist = TargetLocation - GetActorLocation();
	VectorDist = -VectorDist;
	VectorDist.Normalize();
	FVector VelocityDesired = VectorDist * MaxSpeed;
	return VelocityDesired - Velocity;
}

FVector AVehiculeIA::PursuitVelocity()
{
	float Dot = FVector::DotProduct(Target->GetVelocity().GetSafeNormal(), (GetActorLocation() - Target->GetActorLocation()).GetSafeNormal());

	float TurningParameter;
	if (Dot >= 0) TurningParameter = 1 - Dot;
	else TurningParameter = 1;

	float Distance = (Target->GetActorLocation() - GetActorLocation()).Size();
	float T = Distance / Velocity.Size() * TurningParameter;
	FVector FuturTarget = Target->GetVelocity() * T;
	return SeekVelocity(Target->GetActorLocation() + FuturTarget);
}

FVector AVehiculeIA::EvadeVelocity(float DeltaTime)
{
	/*
	if (Time <= 0)
	{
		IsNew = false;

		float Distance = (Target->GetActorLocation() - GetActorLocation()).Size();

		if (Distance <= DistanceChange)
		{
			Time = FMath::FRandRange(0.5f, 2.5f);
			IsNew = true;
			bool IsLeft = FMath::RandBool();
			if (IsLeft)
			{
				FVector Location = GetActorLocation();
				NewTargetEvade = FVector(Location.X + 100, Location.Y, Location.Z);
			}
			else
			{
				FVector Location = GetActorLocation();
				NewTargetEvade = FVector(Location.X - 100, Location.Y, Location.Z);
			}

		}
	}
	else Time -= DeltaTime;

	if(IsNew) return FleeVelocity(NewTargetEvade);

	return FleeVelocity(Target->GetActorLocation());
	*/

	return -PursuitVelocity();
}

void AVehiculeIA::ChangeAlgo()
{
	Index = (Index + 1) % 5;
	UsingAlgo = IntToEnum();
}

AlgoUsing AVehiculeIA::IntToEnum()
{
	switch (Index)
	{
	case 0:
		return AlgoUsing::SEEK;
	case 1:
		return AlgoUsing::FLEE;
	case 2:
		return AlgoUsing::ARRIVAL;
	case 3:
		return AlgoUsing::PURSUIT;
	default:
		return AlgoUsing::EVADE;
	}
}

FText AVehiculeIA::GetNameAlgo()
{
	switch (UsingAlgo)
	{
	case AlgoUsing::SEEK:
		return FText::FromString(TEXT("Seek"));
	case AlgoUsing::FLEE:
		return FText::FromString(TEXT("Flee"));
	case AlgoUsing::ARRIVAL:
		return FText::FromString(TEXT("Arrival"));
	case AlgoUsing::PURSUIT:
		return FText::FromString(TEXT("Pursuit"));
	default:
		return FText::FromString(TEXT("Evade"));
	}
}
