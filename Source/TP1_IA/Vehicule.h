// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TP1_IA_API Vehicule
{
public:
	Vehicule();
	~Vehicule();

protected:
	double mass;
	FVector position;
	FVector velocity;
	FMatrix orientation;
};
