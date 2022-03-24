// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PathFindingPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TP1_IA_API APathFindingPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UHUDPath> BP_PathWidjet;

	void UpdateAlgo(FText Value);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class UHUDPath* HudPath;
};
