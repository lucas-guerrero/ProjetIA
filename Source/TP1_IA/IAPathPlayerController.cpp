// Fill out your copyright notice in the Description page of Project Settings.


#include "IAPathPlayerController.h"
#include "HUDUserWidget.h"

void AIAPathPlayerController::UpdateTextAlgo(FText Value)
{
	if(HudWidget != nullptr) HudWidget->UpdateAlgo(Value);
}

void AIAPathPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (BP_HubWidget != nullptr)
	{
		HudWidget = CreateWidget<UHUDUserWidget>(this, BP_HubWidget);
		//HudWidget->AddToViewport();
	}
}
