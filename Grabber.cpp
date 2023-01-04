// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UPhysicsHandleComponent *PhysicsHandle = GetPhysicsHandle();
	// if(PhysicsHandle==nullptr)
	// {
	// 	return;
	// }
	if(PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector()*HoldRadius;
	  	PhysicsHandle->SetTargetLocationAndRotation(TargetLocation,GetComponentRotation());
	}
}

void UGrabber::Grab()
{

	UPhysicsHandleComponent *PhysicsHandle = GetPhysicsHandle();
	if(PhysicsHandle==nullptr)
	{
		return;
	}
	FHitResult HitResult;
	bool HasHit=GetGrabbableInReach(HitResult);
	if (HasHit)
	{
		AActor *HitActor=HitResult.GetActor();
		UPrimitiveComponent* HitComponent=HitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();
		HitActor->Tags.Add("Grabbed");
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		UE_LOG(LogTemp, Display, TEXT("Hit Actor %s"),*HitActor->GetActorNameOrLabel());
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			HitComponent->GetComponentRotation()
		);
	}
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandle=GetPhysicsHandle();
	// if(PhysicsHandle==nullptr)
	// {
	// 	return;
	// }
	if(PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		PhysicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
		GrabbedActor->Tags.Remove("Grabbed");
		PhysicsHandle->ReleaseComponent();
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result= GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(Result==nullptr)
	{
		UE_LOG(LogTemp, Error , TEXT("Physics Handle is not present"));
	}
		return Result;
}

bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector start=GetComponentLocation();
	FVector end=start+GetForwardVector()*MaxGrabDistance;

	DrawDebugLine(GetWorld(),start,end,FColor::Green);
	DrawDebugSphere(GetWorld(),end,10,10,FColor::Red,false,5);
	return GetWorld()->SweepSingleByChannel(
		OutHitResult,
		start,
		end,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(GrabRadius));
}
 