// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavMeshBoundsVolume.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "EngineUtils.h"
#include "../GameModes/ActorPool.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ATile::SetPool(UActorPool* pool)
{
    if (pool)
    {
        FString name = pool->GetName();
        UE_LOG(LogTemp, Warning, TEXT("Setting pool: %s"), *name);
        ActorPool = pool;
        PositionNavMeshBoundsVolume();
    }
}

void ATile::PositionNavMeshBoundsVolume()
{
    CheckoutOutNavMeshBoundsVolume = Cast<ANavMeshBoundsVolume>(ActorPool->Checkout());

    if (CheckoutOutNavMeshBoundsVolume)
    {
        FVector location = GetActorLocation();
        location.X += Radius();
        CheckoutOutNavMeshBoundsVolume->SetActorLocation(location);
        GetWorld()->GetNavigationSystem()->Build();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Not enough actors in pool"));
    }
}

void  ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    UE_LOG(LogTemp, Warning, TEXT("End Play: %s"), *GetName());

    if (ActorPool)
    {
        ActorPool->Return(CheckoutOutNavMeshBoundsVolume);
    }

    CheckoutOutNavMeshBoundsVolume = nullptr;
}

static UActorComponent* GetActorFromArray(const TArray<UActorComponent*>& actorComponents, FString name)
{
    UActorComponent* const* result = actorComponents.FindByPredicate([name](UActorComponent* uac) { return uac->GetName().Equals( name ); });
    return result ? *result : nullptr;
}


bool ATile::GetEmptyLocation(float radius, FVector min, FVector max, FVector& result)
{
    for (size_t i = 0; i < 10; i++)
    {
        FVector spawnPoint = FMath::RandPointInBox(FBox(min, max));
        FVector worldSpawnPoint = GetTransform().TransformPosition(spawnPoint);

        if (!CastSphere(worldSpawnPoint, radius))
        {
            result = spawnPoint;
            return true;
        }
    }

    return false;
}

float ATile::Radius()
{
    FBox box = GetTerrainBox();
    return 0.5 * (box.Max.X - box.Min.X);
}

FBox ATile::GetTerrainBox()
{
    const TArray<UActorComponent*> components = GetComponentsByClass(UStaticMeshComponent::StaticClass());
    const UStaticMeshComponent* terrain = Cast<const UStaticMeshComponent>(GetActorFromArray(components, FString("Ground")));
    FVector min, max;

    terrain->GetLocalBounds(min, max);
    auto radius = 0.5 * (max.X - min.X);
    min.X += radius;
    max.X += radius;
    max.Z = min.Z = 0;

    return FBox(min, max);
}

void ATile::PlaceActors(TSubclassOf<AActor> toSpawn, int minSpawn, int maxSpawn, float clearance, float minScale, float maxScale)
{
    if (minSpawn >= 1 && maxSpawn >= minSpawn)
    {
        FVector location = this->GetActorLocation();
        FBox box = GetTerrainBox();
        for (size_t i = 0; i < FMath::RandRange(minSpawn, maxSpawn); i++) PlaceActor(toSpawn, box.Min, box.Max, clearance, minScale, maxScale);
    }
}

void ATile::PlaceActor(TSubclassOf<AActor> &toSpawn, const FVector &min, const FVector &max, float clearance, float minScale, float maxScale)
{
    FVector spawnPoint;
    AActor* spawned = GetWorld()->SpawnActor<AActor>(toSpawn);

    if (spawned)
    {
        float randomScale = FMath::RandRange(minScale, maxScale);
        float radius = spawned->GetSimpleCollisionRadius() * randomScale + clearance;

        if (GetEmptyLocation(radius, min, max, spawnPoint))
        {
            spawned->SetActorScale3D(FVector(randomScale));
            float rotation = FMath::RandRange(-180.0f, 180.0f);
            FRotator Rotation(0.0f, 0.0f, 0.0f);
            spawned->SetActorRelativeLocation(spawnPoint);
            spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
            spawned->SetActorRotation(FRotator(0, rotation, 0));
        }
        else
        {
            spawned->Destroy();
        }
    }
}

void ATile::PlaceAIPawns(TSubclassOf<APawn> toSpawn, int minSpawn, int maxSpawn, float clearance)
{
    if (minSpawn >= 1 && maxSpawn >= minSpawn)
    {
        FVector location = this->GetActorLocation();
        FBox box = GetTerrainBox();
        for (size_t i = 0; i < FMath::RandRange(minSpawn, maxSpawn); i++) PlaceAIPawn(toSpawn, box.Min, box.Max, clearance);
    }
}

void ATile::PlaceAIPawn(TSubclassOf<APawn> &toSpawn, const FVector &min, const FVector &max, float clearance)
{
    FVector spawnPoint;
    FVector spawnLocationTemp = FVector(0.0, 0.0, 1000.0);
    FRotator spawnRotationTemp(0.0f, 0.0f, 0.0f);
    APawn* spawned = GetWorld()->SpawnActor<APawn>(toSpawn, spawnLocationTemp, spawnRotationTemp);

    if (spawned)
    {
        float radius = spawned->GetSimpleCollisionRadius() + clearance;

        if (GetEmptyLocation(radius, min, max, spawnPoint))
        {
            float rotation = FMath::RandRange(-180.0f, 180.0f);
            FRotator Rotation(0.0f, 0.0f, 0.0f);
            spawnPoint.Z += 100.0;
            spawned->SetActorRelativeLocation(spawnPoint);
            spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
            spawned->SetActorRotation(FRotator(0, rotation, 0));
            spawned->SpawnDefaultController();
            spawned->Tags.Add(FName("IsEnemy"));
        }
        else
        {
            spawned->Destroy();
        }
    }
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}
//
//// Called every frame
//void ATile::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

bool ATile::CastSphere(FVector location, float radius)
{
    FHitResult hitResult;

    bool hasHit = GetWorld()->SweepSingleByChannel(
        hitResult, 
        location, 
        location, 
        FQuat::Identity, 
        ECollisionChannel::ECC_GameTraceChannel2,
        FCollisionShape::MakeSphere(radius)
    );

    FColor resultColor = hasHit ? FColor::Red : FColor::Green;
    //DrawDebugSphere(GetWorld(), location, radius, 40, resultColor, true, 100);
    DrawDebugCapsule(GetWorld(), location, 0, radius, FQuat::Identity, resultColor, true, 1000);
    return hasHit;
}

