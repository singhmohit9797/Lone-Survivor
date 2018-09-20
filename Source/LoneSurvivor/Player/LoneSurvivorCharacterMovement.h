// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LoneSurvivorCharacterMovement.generated.h"

/**
 * 
 */
UCLASS()
class LONESURVIVOR_API ULoneSurvivorCharacterMovement : public UCharacterMovementComponent
{
	GENERATED_BODY()

	/* Initialise the component */
	ULoneSurvivorCharacterMovement(const FObjectInitializer& ObjectInitializer);

	/* Get the max speed of the charactr based on the current movement */
	virtual float GetMaxSpeed() const override;

};
