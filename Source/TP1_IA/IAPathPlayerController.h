// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IAPathPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TP1_IA_API AIAPathPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UHUDUserWidget> BP_HubWidget;

	void UpdateTextAlgo(FText Value);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class UHUDUserWidget* HudWidget;
};
