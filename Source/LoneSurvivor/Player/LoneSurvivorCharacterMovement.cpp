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
		//Reduce the speed when the player is hurt
		const float CurrentToMaxHealthRatio = Player->GetCurrentHealth() / Player->GetMaxHealth();

		if (Player->IsProne())
		{
			MaxSpeed *= Player->GetProneSpeedModifier();
		}
		else if (Player->IsCouched())
		{
			MaxSpeed *= Player->GetCrouchSpeedModifier();
		}
		else if (Player->IsTargeting())
		{
			MaxSpeed *= Player->GetTargetingSpeedModifier();
		}
		else if (Player->IsRunning())
		{
			MaxSpeed *= Player->GetRunningSpeedModifier();
		}

		MaxSpeed *= CurrentToMaxHealthRatio;
	}

	return MaxSpeed;
}