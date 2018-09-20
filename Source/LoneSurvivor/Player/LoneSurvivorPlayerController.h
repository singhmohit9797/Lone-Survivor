// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LoneSurvivorPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LONESURVIVOR_API ALoneSurvivorPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	//Initialise the Player Controller Object
	ALoneSurvivorPlayerController();

	/** sets up input */
	virtual void SetupInputComponent() override;

	/* Are the player inputs allowed */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	bool IsGameInputAllowed() const { return bAllowPlayerActions; }

	/* Is the game paused */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	bool IsGamePaused() const { return bIsGamePaused; }

protected:

private:
	/* When the player presses the Pause Action Button */
	void OnPause();

	/*Un comment this when the invectory system is implemented */
	/* When the player presses the open Inventory Action Button */
	//void OnShowInventory();

public:

protected:
	/* Whether to allow game input or not */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bAllowPlayerActions : 1;

private:
	/* Whether the game is paused or not */
	uint8 bIsGamePaused : 1;
};
