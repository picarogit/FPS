// Fill out your copyright notice in the Description page of Project Settings.

#include "GrassComponent.h"
#include "FPS.h"
#include "Tile.h"

// Sets default values for this component's properties
UGrassComponent::UGrassComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}


// Called when the game starts
void UGrassComponent::BeginPlay()
{
    Super::BeginPlay();

    ATile* tile = Cast<ATile>(GetAttachmentRootActor());
    FBox box = tile->GetTerrainBox();

    for (size_t i = 0; i < SpawnCount; i++)
    {
        FVector Location = FMath::RandPointInBox(box);
        AddInstance(FTransform(Location));
    }
}

// Called every frame
void UGrassComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}