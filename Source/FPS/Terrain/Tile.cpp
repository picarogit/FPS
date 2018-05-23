// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Classes/Components/StaticMeshComponent.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

static UActorComponent* GetActorFromArray(const TArray<UActorComponent*>& actorComponents, FString name)
{
    UActorComponent* const * other = actorComponents.FindByPredicate([name](UActorComponent* uac) { return uac->GetName().Equals( name ); });
    
    if (other)
    {
        return *other;
    }

    return nullptr;
}

FVector ATile::PlaceActors()
{
    FVector location = this->GetActorLocation();
    const TArray<UActorComponent*> components = GetComponentsByClass(UStaticMeshComponent::StaticClass());
    const UStaticMeshComponent* terrain = Cast<const UStaticMeshComponent> (GetActorFromArray(components, FString("Ground"))); //
    FVector min;
    FVector max;
    
    terrain->GetLocalBounds(min, max);
    auto radius = 0.5 * (max.X - min.X);
    min.Z = max.Z + 0.0;
    min += location;
    max += location;
    min.X += radius;
    max.X += radius;
    FVector point = FMath::RandPointInBox(FBox(min, max));

    return point;
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

