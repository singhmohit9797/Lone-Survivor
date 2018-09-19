// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/PickupObject.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class LONESURVIVOR_API AHealthPickup : public APickupObject
{
	GENERATED_BODY()
	
public:
	//Initialise the health pickup
	AHealthPickup();

	/* Handle the collection of the Ammo Pickup Object */
	void OnPickupCollection_Implementation() override;

	/* Return the healing capacity of the pickup */
	FORCEINLINE float GetHealingCapacity() const { return HealingCapacity; }

protected:
	/* Healing capacity of the pickup */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float HealingCapacity;
};
