// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class ANavMeshBoundsVolume;
class UActorPool;
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

    UFUNCTION(BlueprintCallable, Category = "Setup")
    void PlaceAIPawns(TSubclassOf<APawn> toSpawn, int minSpawn = 1, int maxSpawn = 1, float clearance = 0.0);

protected:
	//// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
    FBox GetTerrainBox();

    UFUNCTION(BlueprintCallable, Category = "Pool")
    void SetPool(UActorPool* pool);


private:
    void PlaceActor(TSubclassOf<AActor> &toSpawn, const FVector &min, const FVector &max, float clearance, float minScale, float maxScale);
    void PlaceAIPawn(TSubclassOf<APawn> &toSpawn, const FVector &min, const FVector &max, float clearance);
    bool GetEmptyLocation(float radius, FVector min, FVector max, FVector& result);
    bool CastSphere(FVector location, float radius);
    void PositionNavMeshBoundsVolume();
    float Radius();

    UActorPool* ActorPool = nullptr;
    ANavMeshBoundsVolume* CheckoutOutNavMeshBoundsVolume = nullptr;
};
