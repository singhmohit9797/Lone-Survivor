// Fill out your copyright notice in the Description page of Project Settings.

#include "LoneSurvivorPlayerController.h"
#include "Components/InputComponent.h"

ALoneSurvivorPlayerController::ALoneSurvivorPlayerController()
{
	bAllowPlayerActions = true;
	bIsGamePaused = false;
}

void ALoneSurvivorPlayerController::SetupInputComponent()	
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Pause", IE_Pressed, this, &ALoneSurvivorPlayerController::OnPause);

	//Uncomment this when the inventory system is implemented
	//InputComponent->BindAction("Inventory", IE_Pressed, this, &ALoneSurvivorPlayerController::OnShowInventory);
}

void ALoneSurvivorPlayerController::OnPause()
{

}
