// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPool.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"

// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


AActor* UActorPool::Checkout()
{
    UE_LOG(LogTemp, Warning, TEXT("[%s] Checkout."), *GetName());

    for (auto& e : IsCheckedOut)
    {
        if (!e.Value)
        {
            e.Value = true;
            auto found = Items.Find(e.Key);

            if (found)
            {
                return *found;
            }
        }
    }

    return nullptr;
}

void UActorPool::Return(AActor* actor)
{
    if (actor && IsCheckedOut.Contains(actor->GetName()))
    {
        IsCheckedOut[actor->GetName()] = false;
    }
}

void UActorPool::Add(AActor* actor)
{
    if (actor)
    {
        Items.Add(actor->GetName(), actor);
        IsCheckedOut.Add(actor->GetName(), false);
    }
}
