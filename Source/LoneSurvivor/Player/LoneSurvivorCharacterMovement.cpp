// Fill out your copyright notice in the Description page of Project Settings.

#include "LoneSurvivorCharacterMovement.h"
#include "LoneSurvivorCharacter.h"
#include "Engine.h"

ULoneSurvivorCharacterMovement::ULoneSurvivorCharacterMovement(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

float ULoneSurvivorCharacterMovement::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	ALoneSurvivorCharacter *Player = Cast<ALoneSurvivorCharacter>(PawnOwner);
	if (Player)
	{
		if (Player->IsProne())
		{
			MaxSpeed *= Player->GetProneSpeedModifier();
			const FString msg1 = "Prone Max Speed " + FString::SanitizeFloat(MaxSpeed);
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, msg1, true, FVector2D::UnitVector * 1.5f);
		}
		else if (Player->IsCouched())
		{
			MaxSpeed *= Player->GetCrouchSpeedModifier();
			const FString msg1 = "Crouched Max Speed " + FString::SanitizeFloat(MaxSpeed);
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, msg1, true, FVector2D::UnitVector * 1.5f);
		}
		else if (Player->IsTargeting())
		{
			MaxSpeed *= Player->GetTargetingSpeedModifier();
			const FString msg1 = "Targetting Max Speed " + FString::SanitizeFloat(MaxSpeed);
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, msg1, true, FVector2D::UnitVector * 1.5f);
		}
		else if (Player->IsRunning())
		{
			MaxSpeed *= Player->GetRunningSpeedModifier();
			const FString msg1 = "Running Max Speed " + FString::SanitizeFloat(MaxSpeed);
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, msg1, true, FVector2D::UnitVector * 1.5f);
		}
	}

	return MaxSpeed;
}