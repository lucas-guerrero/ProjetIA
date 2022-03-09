// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiculePath.h"
#include "GenerateLevels.h"
#include "Destination.h"

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
	IsArrival = false;
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
	if (NoDestination) return;

	ChangeTargetOne();

	FVector SteeringDirection;

	FVector TargetPath = ListPoint[IndexList];

	if (IsArrival) SteeringDirection = ArrivalVelocity(TargetPath);
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
	if (IsArrival)
	{
		FVector NewDest = ListPoint.Top();
		if ((NewDest - GetActorLocation()).Size() < 10.f) NoDestination = true;
		return;
	}

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
	if (!Levels) return;
	FHitResult HitResult;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic, false, HitResult);

	FVector Click = HitResult.Location * 4.75;

	FIntVector Coord = Levels->ClickInPosition(Click);
	if(Levels->IsValid(Coord.X, Coord.Y)) Destination = Coord;

	if (!CompareFIntVector(Destination, Depart))
	{
		GenerateWay();
		GenerateDestination(Destination);
	}
}

void AVehiculePath::GenerateDestination(FIntVector Location)
{
	if (!DestinationClass) return;

	FVector LocationWorld = Levels->GetCoordonne(Location.X, Location.Y);
	LocationWorld.Z = 0.f;
	FTransform SpawnTransform(GetActorRotation(), LocationWorld);
	GetWorld()->SpawnActor<ADestination>(DestinationClass, SpawnTransform);

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Spawn"));
}

void AVehiculePath::GenerateWay()
{
	Levels->ClearMapAlgo();
	bool IsFind = false;
	TArray<FIntVector> ListAVerif;
	ListAVerif.Add(Depart);
	struct Tile &TileDepart = Levels->GetTile(Depart.X, Depart.Y);
	TileDepart.FActual = f(Depart, 0);

	while (ListAVerif.Num() > 0 && !IsFind)
	{
		FIntVector Courant = ListAVerif[0];
		ListAVerif.RemoveAt(0);
		int x = Courant.X;
		int y = Courant.Y;

		if (CompareFIntVector(Courant, Destination))
		{
			IsFind = true;
			continue;
		}

		struct Tile &TileCourant = Levels->GetTile(x, y);
		TileCourant.IsTraitment = true;
		
		float CostCourant = TileCourant.CostActual;

		FIntVector PosLeft(x, y - 1, 0);
		if (AddToList(x, y - 1, CostCourant, Courant)) ListAVerif.Add(PosLeft);

		FIntVector PosRight(x, y + 1, 0);
		if (AddToList(x, y + 1, CostCourant, Courant)) ListAVerif.Add(PosRight);

		FIntVector PosUp(x - 1, y, 0);
		if (AddToList(x - 1, y, CostCourant, Courant)) ListAVerif.Add(PosUp);

		FIntVector PosDown(x + 1, y, 0);
		if (AddToList(x + 1, y, CostCourant, Courant)) ListAVerif.Add(PosDown);


		ListAVerif.Sort([this](const FIntVector& A, const FIntVector& B) {
			struct Tile TileA = Levels->GetTile(A.X, A.Y);
			struct Tile TileB = Levels->GetTile(B.X, B.Y);

			return TileA.FActual < TileB.FActual;
		});
	}
	if(IsFind) AddWayInParcour();
}

bool AVehiculePath::AddToList(int x, int y, float CostCourant, FIntVector Courant)
{
	bool IsAdding = false;
	struct Tile& Tile = Levels->GetTile(x, y);

	if (!Tile.IsTraitment && Tile.IsWalked)
	{
		FIntVector Pos(x, y, 0);
		float CostTmp = f(Pos, CostCourant);

		if (!Tile.IsInList)
		{
			Tile.IsInList = true;
			IsAdding = true;
		}

		if (Tile.FActual > CostTmp)
		{
			Tile.CostActual = Tile.Cost + CostCourant;
			Tile.FActual = CostTmp;
			Tile.PreviousPoint = Courant;
		}
	}
	return IsAdding;
}

bool AVehiculePath::CompareFIntVector(const FIntVector& A, const FIntVector& B)
{
	return A.X == B.X && A.Y == B.Y;
}

void AVehiculePath::AddWayInParcour()
{
	TArray<FVector> ListTmp;
	FIntVector TmpPoint = Destination;
	while (!CompareFIntVector(Depart, TmpPoint))
	{
		FVector LocationPoint = Levels->GetCoordonne(TmpPoint.X, TmpPoint.Y);
		ListTmp.Insert(LocationPoint, 0);

		TmpPoint = Levels->GetTile(TmpPoint.X, TmpPoint.Y).PreviousPoint;
	}

	if (NoDestination)
	{
		ListPoint = ListTmp;
		ReloadNewWay();
	}
	else ListOtherPoint.Add(ListTmp);

	Depart = Destination;
}

float AVehiculePath::f(FIntVector Point, float CostCourant)
{
	struct Tile &TileCourant = Levels->GetTile(Point.X, Point.Y);

	float DistanceTile = Distance(Point);

	float G = TileCourant.Cost + CostCourant;

	return G + DistanceTile;
}

float AVehiculePath::Distance(FIntVector Point)
{
	int XDest = Destination.X;
	int YDest = Destination.Y;

	int XPoint = Point.X;
	int YPoint = Point.Y;

	return abs(XDest - XPoint) + abs(YDest - YPoint);
}

void AVehiculePath::SwapWay()
{
	if (ListOtherPoint.Num() > 0)
	{
		ListPoint = ListOtherPoint[0];
		ListOtherPoint.RemoveAt(0);

		ReloadNewWay();
	}
}

void AVehiculePath::ReloadNewWay()
{
	IndexList = 0;
	NoDestination = false;
	IsArrival = false;
}