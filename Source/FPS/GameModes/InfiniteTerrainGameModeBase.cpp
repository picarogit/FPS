// Fill out your copyright notice in the Description page of Project Settings.

#include "InfiniteTerrainGameModeBase.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavMeshBoundsVolume.h"
#include "ActorPool.h"


AInfiniteTerrainGameModeBase::AInfiniteTerrainGameModeBase()
{
    ActorPool = CreateDefaultSubobject<UActorPool>(FName("Nav Mesh Bounds Volumn Pool"));
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

