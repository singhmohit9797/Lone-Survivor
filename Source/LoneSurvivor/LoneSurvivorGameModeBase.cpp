// Fill out your copyright notice in the Description page of Project Settings.

#include "LoneSurvivorGameModeBase.h"
#include "Player/LoneSurvivorPlayerController.h"
#include "UObject/ConstructorHelpers.h"

ALoneSurvivorGameModeBase::ALoneSurvivorGameModeBase()
	:Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/Characters/BP_LoneSurvivorCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	PlayerControllerClass = ALoneSurvivorPlayerController::StaticClass();
}