// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Vehicul.generated.h"

UCLASS()
class TP1_IA_API AVehicul : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVehicul();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float Mass;
	float MaxForce;
	float MaxSpeed;

	FVector Velocity;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	FVector Truncate(FVector Vector, float Max);
	FVector SeekVelocity(FVector Target);
};
