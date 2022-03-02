// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiculePath.h"
#include "GenerateLevels.h"

#include <Components/InputComponent.h>
#include <Kismet/GameplayStatics.h>

AVehiculePath::AVehiculePath()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AVehiculePath::BeginPlay()
{
	Super::BeginPlay();

	IndexList = 0;
	IsArrival = true;
	NoDestination = true;

	TArray<AActor*> List;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGenerateLevels::StaticClass(), List);

	if (List.Num() > 0)
	{
		Levels = Cast<AGenerateLevels>(List[0]);
		Depart = Levels->PositionInMap(GetActorLocation());
	}

	BindInput();
}

void AVehiculePath::BindInput()
{
	InputComponent = NewObject<UInputComponent>(this);
	InputComponent->RegisterComponent();
	if (InputComponent)
	{
		InputComponent->BindAction("MouseClick", IE_Pressed, this, &AVehiculePath::Click);

		EnableInput(GetWorld()->GetFirstPlayerController());
	}

	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}

// Called every frame
void AVehiculePath::Tick(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, Destination.ToString());
	if (NoDestination) return;

	FVector SteeringDirection;

	FVector TargetPath = ListPoint[IndexList];

	if (IsArrival) SteeringDirection =  ArrivalVelocity(TargetPath);
	SteeringDirection = SeekVelocity(TargetPath);

	FVector SteeringForce = Truncate(SteeringDirection, MaxForce);
	FVector Acceleration = SteeringForce / Mass;
	Velocity = Truncate(Velocity + Acceleration, MaxSpeed);
	SetActorLocation(GetActorLocation() + Velocity);

	SetActorRotation(FRotator(Velocity.Rotation()));
}

FVector AVehiculePath::CastToInt(FIntVector Vector)
{
	int x = Vector.X;
	int y = Vector.Y;
	return FVector(x, y, 0.f);
}

void AVehiculePath::ChangeTargetOne()
{
	if (IsArrival) return;

	FVector TargetPath = ListPoint[IndexList];
	float Distance = (TargetPath - GetActorLocation()).Size();

	if (Distance <= DistanceChangePoint) ++IndexList;
	if (IndexList >= ListPoint.Num())
	{
		IsArrival = true;
		IndexList = ListPoint.Num() - 1;
	}
}

void AVehiculePath::Click()
{
	FHitResult HitResult;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic, false, HitResult);

	FIntVector Coord;
	if (Levels)
	{
		Coord = Levels->PositionInMap(HitResult.Location * 4.75);
		if(Levels->IsValid(Coord.X, Coord.Y)) Destination = Coord;
	}
	//NoDestination = false;
	GenerateWay();
}

void AVehiculePath::GenerateWay()
{
	if (!Levels) return;

	Levels->ClearMapAlgo();


}

