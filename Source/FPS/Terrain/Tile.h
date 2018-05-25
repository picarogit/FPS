// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

struct FBox;

UCLASS()
class FPS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

    UFUNCTION(BlueprintCallable, Category = "Setup")
    void PlaceActors(TSubclassOf<AActor> toSpawn, int minSpawn = 1, int maxSpawn = 1, float clearance = 0.0, float minScale = 1.0, float maxScale = 1.0);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    FBox GetTerrainBox();

private:
    void PlaceActor(TSubclassOf<AActor> &toSpawn, const FVector &min, const FVector &max, float clearance, float minScale, float maxScale);
    bool GetEmptyLocation(float radius, FVector min, FVector max, FVector& result);
    bool CastSphere(FVector location, float radius);
	
};
