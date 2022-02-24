// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vehicule.h"
#include "VehiculePath.generated.h"

/**
 * 
 */
UCLASS()
class TP1_IA_API AVehiculePath : public AVehicule
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AVehiculePath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
