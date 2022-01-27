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

		int32 Index = FMath::RandRange(0, Vehiculs.Num()-1);
		PursuitActor = Cast<AVehicul>(Vehiculs[Index]);
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

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("SteeringAlgo: %d"), SteeringAlgo));

	FVector SteeringDirection;

	if (SteeringAlgo == SteeringAlgo::SEEK) SteeringDirection = SeekVelocity(TargetActor->GetActorLocation());
	else if (SteeringAlgo == SteeringAlgo::ARRIVAL) SteeringDirection = ArrivalVelocity(TargetActor->GetActorLocation());
	else if (SteeringAlgo == SteeringAlgo::FLEE) SteeringDirection = FleeVelocity(TargetActor->GetActorLocation());
	else SteeringDirection = PursuitVelocity(PursuitActor);

	FVector SteeringForce = Truncate(SteeringDirection, MaxForce);
	FVector Acceleration = SteeringForce / Mass;
	Velocity = Truncate(Velocity + Acceleration, MaxSpeed);
	SetActorLocation(GetActorLocation() + Velocity);

	SetActorRotation(FRotator(Velocity.Rotation()));
}

FVector AVehicul::SeekVelocity(FVector Target)
{
	FVector VectorDist = Target - GetActorLocation();
	VectorDist.Normalize();
	FVector VelocityDesired = VectorDist * MaxSpeed;
	return VelocityDesired - Velocity;
}

FVector AVehicul::ArrivalVelocity(FVector Target)
{
	FVector VectorDist = Target - GetActorLocation();
	float Distance = VectorDist.Size();

	float RampedSpeed = MaxSpeed * (Distance / SlowingDistance);

	float ClippedSpeed = FMath::Min(RampedSpeed, MaxSpeed);

	FVector VelocityDesired = VectorDist * (ClippedSpeed / Distance);
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

FVector AVehicul::PursuitVelocity(AVehicul* Target)
{
	float Distance = (Target->GetActorLocation() - GetActorLocation()).Size();
	float T = Distance * TurningParameter;

	FVector FuturTarget = Target->GetVelocity() * T;

	return SeekVelocity(FuturTarget + Target->GetActorLocation());
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

