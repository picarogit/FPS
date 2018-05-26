// Fill out your copyright notice in the Description page of Project Settings.

#include "InfiniteTerrainGameModeBase.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavMeshBoundsVolume.h"


AInfiniteTerrainGameModeBase::AInfiniteTerrainGameModeBase()
{
    int i = 0;
}

void AInfiniteTerrainGameModeBase::AddToPool(ANavMeshBoundsVolume* actor)
{
    if (actor)
    {
        UE_LOG(LogTemp, Warning, TEXT("Found Actor: %s"), *actor->GetName());
    }
}

void AInfiniteTerrainGameModeBase::PopulateBoundsVolumePool()
{
    TActorIterator<ANavMeshBoundsVolume> actorIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());

    while (actorIterator)
    {
        ANavMeshBoundsVolume* foundActor = *actorIterator;
        AddToPool(foundActor);
        ++actorIterator;
    }
}

