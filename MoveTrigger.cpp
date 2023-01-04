// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveTrigger.h"
#include "GameFramework/Actor.h"

UMoveTrigger::UMoveTrigger()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UMoveTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

void UMoveTrigger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if(Mover==nullptr)
    {
        return;
    }
    
    AActor* UnlockingActor=GetAcceptableActor();
    if(UnlockingActor!=nullptr)
    {
        UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(UnlockingActor->GetRootComponent());
        if(Component!=nullptr)
        {
            Component->SetSimulatePhysics(false);
        }
        UnlockingActor->AttachToComponent(this,FAttachmentTransformRules::KeepWorldTransform);
        Mover->SetShouldMove(true);
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("Nullptr"));
        Mover->SetShouldMove(false);
    }
    
}

void UMoveTrigger::SetMover(UMover* NewMover)
{
    Mover = NewMover;
} 

AActor* UMoveTrigger::GetAcceptableActor() const
{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);
    
    for(AActor* Actor:Actors)
    {
        bool IsGrabbed = Actor->ActorHasTag("Grabbed");
        if((Actor->ActorHasTag(UnlockingActorTag)) && !IsGrabbed)
        {
            return Actor;
        }
    }
    return nullptr;
}
