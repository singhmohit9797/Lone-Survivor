// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoPickup.h"

AAmmoPickup::AAmmoPickup()
	:APickupObject()
{
	//Set the initial value for Ammo Capacity
	AmmoCapacity = 60;

	//Set the Pickup Name
	PickupObjectName = TEXT("Ammo Box");

	//Set the Ammo Type
	AmmoType = EAmmoType::Rifle;
}

void AAmmoPickup::OnPickupCollection_Implementation()
{
	//Call the parent class implementation of the pickup collection
	Super::OnPickupCollection_Implementation();
}