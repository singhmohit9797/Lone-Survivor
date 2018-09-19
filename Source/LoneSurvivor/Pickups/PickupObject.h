// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyObject.h"
#include "PickupObject.generated.h"

/**
 * 
 */
UCLASS()
class LONESURVIVOR_API APickupObject
	: public AMyObject
{
	GENERATED_BODY()
protected:
	/* Whether the pickup object is active */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup State")
	uint8 bIsActive : 1;

public:
	//Initialise the Pickup object
	APickupObject();

	/* Get the state of the pickup */
	UFUNCTION(BlueprintPure, Category = "Pickup State")
	uint8 GetActive() const { return bIsActive; }

	/* Deactivates the pickup object */
	UFUNCTION(BlueprintCallable, Category = "Pickup State")
	void DeactivatePickup();

	UFUNCTION(BlueprintNativeEvent, Category = "Pickup Collection")
	void OnPickupCollection();
	virtual void OnPickupCollection_Implementation();
};
