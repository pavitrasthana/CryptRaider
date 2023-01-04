// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mover.h"
#include "Components/BoxComponent.h"
#include "MoveTrigger.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UMoveTrigger : public UBoxComponent
{
	GENERATED_BODY()

	public:
		virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	public:
		UMoveTrigger();

		UFUNCTION(BlueprintCallable)
		void SetMover(UMover* Mover); 

	protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	private:
		UPROPERTY(EditAnywhere)
		FName UnlockingActorTag;

		UMover* Mover;
		 
		AActor* GetAcceptableActor() const;

};
