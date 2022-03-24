// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDPath.generated.h"

/**
 * 
 */
UCLASS()
class TP1_IA_API UHUDPath : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentAlgo;

	void UpdateAlgo(FText Value);
};
