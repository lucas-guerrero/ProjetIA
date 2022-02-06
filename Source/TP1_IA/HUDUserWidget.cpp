// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDUserWidget.h"

#include <Components/TextBlock.h>

void UHUDUserWidget::UpdateAlgo(FText Value)
{
	if (CurrentAlgo != nullptr)
	{
		CurrentAlgo->SetText(Value);
	}
}