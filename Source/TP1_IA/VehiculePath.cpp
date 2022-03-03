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

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("Index %d, %f"), IndexList, DistanceChangePoint));

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
	GenerateWay();
}

void AVehiculePath::GenerateWay()
{
	Levels->ClearMapAlgo();
	bool IsFind = false;
	TArray<FIntVector> ListAVerif;
	ListAVerif.Add(Depart);
	struct Tile &TileDepart = Levels->GetTile(Depart.X, Depart.Y);
	TileDepart.FActual = f(Depart, 0);

	UE_LOG(LogTemp, Warning, TEXT("Dist x = %d, y = %d"), Destination.X, Destination.Y);

	while (ListAVerif.Num() > 0 && !IsFind)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("SizeList: %d"), ListAVerif.Num()));
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
		
		UE_LOG(LogTemp, Warning, TEXT("Courant:"));
		UE_LOG(LogTemp, Warning, TEXT("\t - x = %d, y = %d"), x, y);
		UE_LOG(LogTemp, Warning, TEXT("\t - Previous: %d, %d"), TileCourant.PreviousPoint.X, TileCourant.PreviousPoint.Y);
		UE_LOG(LogTemp, Warning, TEXT("\t - F: %f"), TileCourant.FActual);
		UE_LOG(LogTemp, Warning, TEXT("\t - Cost: %f"), TileCourant.CostActual);
		
		float CostCourant = TileCourant.CostActual;

		struct Tile &TileLeft = Levels->GetTile(x, y-1);
		
		UE_LOG(LogTemp, Warning, TEXT("Left:"));
		UE_LOG(LogTemp, Warning, TEXT("\t - x = %d, y = %d"), x, y-1);
		
		if (!TileLeft.IsTraitment && TileLeft.IsWalked)
		{
			FIntVector Pos(x, y-1, 0);
			float CostTmp = f(Pos, CostCourant);

			if (!TileLeft.IsInList)
			{
				TileLeft.IsInList = true;
				ListAVerif.Add(Pos);
			}

			if (TileLeft.FActual > CostTmp)
			{
				TileLeft.CostActual = TileLeft.Cost + CostCourant;
				TileLeft.FActual = CostTmp;
				TileLeft.PreviousPoint = Courant;
			}

			UE_LOG(LogTemp, Warning, TEXT("\t - FActual = %f"), TileLeft.FActual);
		}

		struct Tile &TileRight = Levels->GetTile(x, y+1);
		
		UE_LOG(LogTemp, Warning, TEXT("Right:"));
		UE_LOG(LogTemp, Warning, TEXT("\t - x = %d, y = %d"), x, y+1);
		
		if (!TileRight.IsTraitment && TileRight.IsWalked)
		{
			FIntVector Pos(x, y+1, 0);
			float CostTmp = f(Pos, CostCourant);

			if (!TileRight.IsInList)
			{
				TileRight.IsInList = true;
				ListAVerif.Add(Pos);
			}

			if (TileRight.FActual > CostTmp)
			{
				TileRight.CostActual = TileRight.Cost + CostCourant;
				TileRight.FActual = CostTmp;
				TileRight.PreviousPoint = Courant;
			}

			UE_LOG(LogTemp, Warning, TEXT("\t - F = %f"), TileRight.FActual);
		}

		struct Tile &TileUp = Levels->GetTile(x-1, y);
		
		UE_LOG(LogTemp, Warning, TEXT("Up:"));
		UE_LOG(LogTemp, Warning, TEXT("\t - x = %d, y = %d"), x-1, y);
		
		if (!TileUp.IsTraitment && TileUp.IsWalked)
		{
			FIntVector Pos(x-1, y, 0);
			float CostTmp = f(Pos, CostCourant);

			if (!TileUp.IsInList)
			{
				TileUp.IsInList = true;
				ListAVerif.Add(Pos);
			}

			if (TileUp.FActual > CostTmp)
			{
				TileUp.CostActual = TileUp.Cost + CostCourant;
				TileUp.FActual = CostTmp;
				TileUp.PreviousPoint = Courant;
			}
			UE_LOG(LogTemp, Warning, TEXT("\t - F = %f"), TileUp.FActual);
		}

		struct Tile &TileDown = Levels->GetTile(x+1, y);
		
		UE_LOG(LogTemp, Warning, TEXT("Down:"));
		UE_LOG(LogTemp, Warning, TEXT("\t - x = %d, y = %d"), x + 1, y);
		
		if (!TileDown.IsTraitment && TileDown.IsWalked)
		{
			FIntVector Pos(x+1, y, 0);
			float CostTmp = f(Pos, CostCourant);

			if (!TileDown.IsInList)
			{
				TileDown.IsInList = true;
				ListAVerif.Add(Pos);
			}

			if (TileDown.FActual > CostTmp)
			{
				TileDown.CostActual = TileDown.Cost + CostCourant;
				TileDown.FActual = CostTmp;
				TileDown.PreviousPoint = Courant;
			}
			UE_LOG(LogTemp, Warning, TEXT("\t - F = %f"), TileDown.FActual);
		}

		ListAVerif.Sort([this](const FIntVector& A, const FIntVector& B) {
			struct Tile TileA = Levels->GetTile(A.X, A.Y);
			struct Tile TileB = Levels->GetTile(B.X, B.Y);

			return TileA.FActual < TileB.FActual;
		});
	}
	if(IsFind) AddWayInParcour();

	int Z = 1;
	for (auto Coord : ListPoint)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d - %f, %f"), Z, Coord.X, Coord.Y);
		++Z;
	}
}

bool AVehiculePath::CompareFIntVector(const FIntVector& A, const FIntVector& B)
{
	if (A.X != B.X) return false;
	if (A.Y != B.Y) return false;
	return true;
}

void AVehiculePath::AddWayInParcour()
{
	NoDestination = false;
	FIntVector TmpPoint = Destination;
	while (!CompareFIntVector(Depart, TmpPoint))
	{
		FVector LocationPoint = Levels->GetCoordonne(TmpPoint.X, TmpPoint.Y);
		ListPoint.Insert(LocationPoint, 0);

		struct Tile TileTmp = Levels->GetTile(TmpPoint.X, TmpPoint.Y);
		TmpPoint = TileTmp.PreviousPoint;
	}
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
