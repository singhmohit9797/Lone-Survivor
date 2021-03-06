// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/PickupObject.h"
#include "AmmoPickup.generated.h"

UENUM()
enum class EAmmoType : uint8
{
	Handgun,
	Rifle
};

/**
 * 
 */
UCLASS()
class LONESURVIVOR_API AAmmoPickup : public APickupObject
{
	GENERATED_BODY()
	
public:
	/* Initialise the AmmoPickup object */
	AAmmoPickup();

	/* Handle the collection of the Ammo Pickup Object */
	void OnPickupCollection_Implementation() override;
	
	/* Returns the ammo capacity of the pickup */
	FORCEINLINE int32 GetAmmoCapacity() const { return AmmoCapacity; }

	/* Returns the ammo type in the pickup */
	FORCEINLINE EAmmoType GetAmmoType() const{ return AmmoType; }

protected:
	/* Ammo available in the pickup */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int32 AmmoCapacity;

	/* Type of the ammo in the pickup object */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	EAmmoType AmmoType;
};
