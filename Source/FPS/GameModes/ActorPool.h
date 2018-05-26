// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/Templates/Tuple.h"
#include "Components/ActorComponent.h"
#include "ActorPool.generated.h"


USTRUCT()
struct FMeshAndStatus
{
    GENERATED_USTRUCT_BODY()

    AActor* Item;
    bool IsCheckedOut;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPS_API UActorPool : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorPool();

    AActor* Checkout();
    void Return(AActor* actor);
    void Add(AActor* actor);

private:

    UPROPERTY()
    TMap<FString, FMeshAndStatus> Items;
};
