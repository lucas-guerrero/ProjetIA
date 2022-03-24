// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFindingPlayerController.h"
#include "HUDPath.h"


void APathFindingPlayerController::UpdateAlgo(FText Value)
{
	if (!HudPath) return;
	HudPath->UpdateAlgo(Value);
}

void APathFindingPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!BP_PathWidjet) return;

	HudPath = CreateWidget<UHUDPath>(this, BP_PathWidjet);
	HudPath->AddToViewport();
}