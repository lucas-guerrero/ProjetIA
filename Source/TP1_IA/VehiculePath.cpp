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
	if (!Levels) return;
	FHitResult HitResult;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic, false, HitResult);

	FVector Click = HitResult.Location * 4.75;

	FIntVector Coord = Levels->ClickInPosition(Click);
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Click %d : %d"), Coord.X, Coord.Y));
	if(Levels->IsValid(Coord.X, Coord.Y)) Destination = Coord;
	//NoDestination = false;
	GenerateWay();
}

void AVehiculePath::GenerateWay()
{
	Levels->ClearMapAlgo();
	bool IsFind = false;
	TArray<FIntVector> ListAVerif;
	ListAVerif.Add(Depart);
	struct Tile &TileDepart = Levels->GetTile(Depart.X, Depart.Y);
	//TileDepart.IsTraitment = false;
	TileDepart.CostActual = f(Depart, 0);

	while (ListAVerif.Num() > 0 && !IsFind)
	{
		FIntVector Courant = ListAVerif.Top();
		ListAVerif.RemoveAt(0);
		int x = Courant.X;
		int y = Courant.Y;

		if (Courant == Destination)
		{
			IsFind = true;
			continue;
		}

		struct Tile TileCourant = Levels->GetTile(x, y);
		TileCourant.IsTraitment = true;

		float CostCourant = TileCourant.CostActual;

		struct Tile &TileLeft = Levels->GetTile(x-1, y);
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Tile Left: CostActual = %f"), TileLeft.CostActual));
		if (!TileLeft.IsTraitment && TileLeft.IsWalked)
		{
			FIntVector Pos(x - 1, y, 0);
			float CostTmp = f(Pos, CostCourant);

			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Tile Left: CostActual = %f, f = %f"), TileLeft.CostActual, CostTmp));

			if (!TileLeft.IsInList)
			{
				TileLeft.IsInList = true;
				//ListAVerif.Add(Pos);
			}

			if (TileLeft.CostActual > CostTmp)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Victoire !!!")));
				TileLeft.CostActual = CostTmp;
				TileLeft.PreviousPoint = Courant;
			}

		}

		struct Tile TileRight = Levels->GetTile(x+1, y);
		struct Tile TileUp = Levels->GetTile(x, y-1);
		struct Tile TileDown = Levels->GetTile(x, y+1);


	}

	struct Tile TileLeft = Levels->GetTile(Depart.X-1, Depart.Y);

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Tile Left: CostActual = %d"), TileLeft.CostActual));
	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TileLeft.PreviousPoint.ToString());

	if (TileLeft.IsInList)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("2 ieme VICTOIRE !!!!")));
	}
}

bool AVehiculePath::AddToList(int x, int y, float CostCourant, FIntVector PointCourant)
{
	bool IsList = false;
	struct Tile TileLeft = Levels->GetTile(x, y);
	if (!TileLeft.IsTraitment && TileLeft.IsWalked)
	{
		FIntVector Pos(x, y, 0);
		float CostTmp = f(Pos, CostCourant);

		if (!TileLeft.IsInList)
		{
			TileLeft.IsInList = true;
			IsList = true;
		}

		if (TileLeft.CostActual > CostTmp)
		{
			TileLeft.CostActual = CostTmp;
			TileLeft.PreviousPoint = PointCourant;
		}
	}
	return IsList;
}

float AVehiculePath::f(FIntVector Point, float CostCourant)
{
	struct Tile TileCourant = Levels->GetTile(Point.X, Point.Y);

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
