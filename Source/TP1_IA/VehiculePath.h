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

	class AGenerateLevels* Levels;

	FVector Destination;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void BindInput();

public:
	virtual void Tick(float DeltaTime) override;

	void Click();
};
