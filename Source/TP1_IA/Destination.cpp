// Fill out your copyright notice in the Description page of Project Settings.


#include "Destination.h"
#include "VehiculePath.h"

#include <Components/BoxComponent.h>

ADestination::ADestination()
{
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(RootComponent);
}

void ADestination::BeginPlay()
{
	Super::BeginPlay();

	//CollisionBox = Cast<UBoxComponent>(this->GetComponentByClass(UBoxComponent::StaticClass()));

	if(CollisionBox) CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ADestination::OnOverlapBegin);
}

void ADestination::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AVehiculePath* Player = Cast<AVehiculePath>(OtherActor);
	if (Player)
	{
		Player->SwapWay();
		Destroy();
	}
}