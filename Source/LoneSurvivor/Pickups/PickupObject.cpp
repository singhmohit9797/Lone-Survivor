// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupObject.h"
#include "Engine.h"
#include "Components/StaticMeshComponent.h"

APickupObject::APickupObject()
	: AMyObject()
{
	//Set as active by default
	bIsActive = true;

	GetObjectMesh()->SetSimulatePhysics(false);
}

//Deactivate the pickup
void APickupObject::DeactivatePickup()
{
	bIsActive = false;
}

void APickupObject::OnPickupCollection_Implementation()
{
	//Display the debug message on the screen
	const FString DebugMessage = "Picked up " + GetName();
	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, DebugMessage, true, FVector2D::UnitVector * 1.5);

	//Mark to object to destroyed in the next frame
	Destroy();
}

