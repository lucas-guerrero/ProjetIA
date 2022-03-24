// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDPath.h"

#include <Components/TextBlock.h>

void UHUDPath::UpdateAlgo(FText Value)
{
	if (!CurrentAlgo) return;
	CurrentAlgo->SetText(Value);
}