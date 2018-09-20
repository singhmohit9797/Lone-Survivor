// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LoneSurvivorCharacter.generated.h"

namespace EPlayerStance
{
	enum Stance
	{
		Standing,
		Crouch,
		Prone,
	};
}

UCLASS()
class LONESURVIVOR_API ALoneSurvivorCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALoneSurvivorCharacter(const FObjectInitializer& ObjectInitializer);

	/** spawn inventory, setup initial variables */
	virtual void PostInitializeComponents() override;

	virtual void Destroyed() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Return whether the player is alive or not */
	FORCEINLINE bool IsAlive() const { return CurrentHitPoints > 0; }

	/* Get the player mesh */
	FORCEINLINE USkeletalMeshComponent* GetPlayerMesh() const { return PlayerMesh1P; }

	/* If the player can reload or not */
	bool CanReload() const;

	/* If the player can fire or not */
	bool CanFire() const;

	/* Get if the player is targeting */
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	bool IsTargeting() const { return bIsTargeting; }

	/* Get if the Player if firing */
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	bool IsFiring() const { return bIsFiring; }

	/* Get if the player is running */
	UFUNCTION(BlueprintCallable, Category = "Game|Movement")
	bool IsRunning() const { return bIsRunning; }

	/* Get if the player is in crouch stance */
	UFUNCTION(BlueprintCallable, Category = "Game|Movement")
	bool IsCouched() const { return bIsCrouched; }

	/* Get if the player is prone stance */
	UFUNCTION(BlueprintCallable, Category = "Game|Movement")
	bool IsProne() const { return bIsProne; }

	/* Get Targeting speed modifier */
	UFUNCTION(BlueprintCallable, Category = "Game|Movement")
	float GetTargetingSpeedModifier() const { return TargetingSpeedModifier; }

	/* Get Running speed modifier */
	UFUNCTION(BlueprintCallable, Category = "Game|Movement")
	float GetRunningSpeedModifier() const { return RunningSpeedModifier; }

	/* Get Crouch speed modifier */
	UFUNCTION(BlueprintCallable, Category = "Game|Movement")
	float GetCrouchSpeedModifier() const { return CrouchSpeedModifier; }

	/* Get Prone speed modifier */
	UFUNCTION(BlueprintCallable, Category = "Game|Movement")
	float GetProneSpeedModifier() const { return ProneSpeedModifier; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	/* Triggered when player presses Jump Action Button */
	void OnJumpStart();

	/* Triggered when player releases Jump Action Button */
	void OnJumpStop();

	/* Triggered when player presses Sprint / Run Action Button */
	void OnStartRunning();

	/* Triggered when player releases Sprint / Run Action Button */
	void OnStopRunning();

	/* Triggered when player presses Toggle Crouch Action Button */
	void ToggleCrouch();

	/* Triggered when player presses Toggle Prone Action Button */
	void ToggleProne();

	/* Triggered when player presses Fire Action Button */
	void OnStartFire();

	/* Triggered when player releases Fire Action Button */
	void OnStopFire();

	/* Triggered when player presses Aim Down Sight Action Button */
	void OnStartTargeting();

	/* Triggered when player releases Aim Down Sight Action Button */
	void OnStopTargeting();

	/**
	* Move forward/back
	*
	* @param Value Movment input to apply
	*/
	void MoveForward(const float Value);
	
	/**
	* Strafe right/left
	*
	* @param Value Movment input to apply
	*/
	void MoveRight(const float Value);

	/* Frame rate independent turn */
	void TurnAtRate(const float Value);

	/* Triggered when player presses Interact Action Button */
	void Interact();

	/* Triggered when player presses Reload Action Button */
	void OnReload();

	/* Set new Targeting state */
	void SetTargeting(bool bNewTargeting);

	/* Set new running state */
	void SetRunning(bool bNewRunning);

	/* Set new firing state */
	void SetFiring(bool bNewFiring);

	/* Set new Stance */
	void SetStance(EPlayerStance::Stance bNewStance);

public:



protected:
	/* Base Turn Rate */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BaseTurnRate;

	/* Base LookUp Rate*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BaseLookUpRate;

	/* Maximum Health of the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float MaxHitPoints;

	/* Speed multiplier used when the player is running / sprinting */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Movement")
	float RunningSpeedModifier;

	/* Speed multiplier used when the player is targeting (Aiming Down Sight) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Movement")
	float TargetingSpeedModifier;

	/* Speed Multiplier used when the player is in Crouch Position */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Movement")
	float CrouchSpeedModifier;

	/* Speed Multiplier used when the player is Prone */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Movement")
	float ProneSpeedModifier;

private:
	/* Whether player is jumping */
	uint8 bIsJumping : 1;

	/* Whether player is running */
	uint8 bIsRunning : 1;

	/* Current Posture of the player i.e. whether the player is in Standing Position, Crouch Position or Prone Position */
	EPlayerStance::Stance CurrentStance;

	/* WHether the player is firing */
	uint8 bIsFiring : 1;

	/* Whether player is aiming down the sight */
	uint8 bIsTargeting : 1;

	/* Whether the player is in Prone stance */
	uint8 bIsProne : 1;

	/* Whether the player is in crouch stance */
	uint8 bIsCrouched : 1;

	/* Current Health of the player */
	float CurrentHitPoints;

	/* Default eye height in Prone stance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera", META = (AllowPrivateAccess = "true"))
	float ProneEyeHeight;

	/* Skeletal Mesh to represent the player in the world */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", META = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent *PlayerMesh1P;

	/* First Person Camera Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", META = (AllowPrivateAccess = "true"))
	class UCameraComponent *FPSCamera;
};
