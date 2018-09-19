// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthPickup.h"

AHealthPickup::AHealthPickup()
{
	//Initial value of the pickup
	HealingCapacity = 50.f;
}

void AHealthPickup::OnPickupCollection_Implementation()
{
	Super::OnPickupCollection_Implementation();
}