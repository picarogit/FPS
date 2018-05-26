// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSGameMode.h"
#include "InfiniteTerrainGameModeBase.generated.h"


class ANavMeshBoundsVolume;

/**
 * 
 */
UCLASS()
class FPS_API AInfiniteTerrainGameModeBase : public AFPSGameMode
{
	GENERATED_BODY()
	
public:

    AInfiniteTerrainGameModeBase();

    UFUNCTION(BlueprintCallable, Category = "Bounds Pool")
    void PopulateBoundsVolumePool();


private:

    void AddToPool(ANavMeshBoundsVolume* volume);

};
