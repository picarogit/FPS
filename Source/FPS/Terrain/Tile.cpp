// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

static UActorComponent* GetActorFromArray(const TArray<UActorComponent*>& actorComponents, FString name)
{
    UActorComponent* const* result = actorComponents.FindByPredicate([name](UActorComponent* uac) { return uac->GetName().Equals( name ); });
    
    if (result)
    {
        return *result;
    }

    return nullptr;
}

bool ATile::GetEmptyLocation(float radius, FVector min, FVector max, FVector& result)
{
    for (size_t i = 0; i < 10; i++)
    {
        FVector spawnPoint = FMath::RandPointInBox(FBox(min, max));
        
        if (CastSphere(spawnPoint, radius))
        {
            result = spawnPoint;
            return true;
        }
    }

    return false;
}

void ATile::PlaceActors(TSubclassOf<AActor> toSpawn, int minSpawn, int maxSpawn, float radius)
{
    if (minSpawn >= 1 && maxSpawn >= minSpawn)
    {
        FVector location = this->GetActorLocation();
        const TArray<UActorComponent*> components = GetComponentsByClass(UStaticMeshComponent::StaticClass());
        const UStaticMeshComponent* terrain = Cast<const UStaticMeshComponent>(GetActorFromArray(components, FString("Ground")));
        FVector min, max;

        terrain->GetLocalBounds(min, max);
        auto radius = 0.5 * (max.X - min.X);

        min.X += radius;
        max.X += radius;
        max.Z = 0;
        min.Z = 0;

        int numberToSpawn = FMath::RandRange(minSpawn, maxSpawn);
        for (size_t i = 0; i < numberToSpawn; i++)
        {
            FVector spawnPoint;

            if (GetEmptyLocation(radius, min, max, spawnPoint))
            {
                AActor* spawned = GetWorld()->SpawnActor<AActor>(toSpawn);
                float collisionRadius = spawned->GetSimpleCollisionRadius();
                FRotator Rotation(0.0f, 0.0f, 0.0f);
                spawned->SetActorRelativeLocation(spawnPoint);
                spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
            }
            else
            {
            }
        }
    }
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

    CastSphere(GetActorLocation(), 300.0);
	
    CastSphere(GetActorLocation() + FVector(0, 0, 1000), 300);
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

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

