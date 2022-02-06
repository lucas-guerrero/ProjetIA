// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Vehicule.h"
#include "VehiculePlayer.generated.h"

/**
 * 
 */
UCLASS()
class TP1_IA_API AVehiculePlayer : public AVehicule
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AVehiculePlayer();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	AVehicule* Vehicule;

	FVector Direction;

	void BindInput();

public:
	virtual void Tick(float DeltaTime) override;

	void Change();
	void Quit();
	void MoveForward(float Value);
	void MoveRight(float Value);
	void MoveUp(float Value);
};
