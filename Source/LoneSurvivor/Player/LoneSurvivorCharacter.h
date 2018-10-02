// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LoneSurvivorCharacter.generated.h"

UENUM()
enum class EPlayerStance
{
	Standing,
	Crouch,
	Prone,
};

UCLASS()
class LONESURVIVOR_API ALoneSurvivorCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALoneSurvivorCharacter(const FObjectInitializer& ObjectInitializer);

	/** spawn inventory, setup initial variables */
	virtual void PostInitializeComponents() override;

	/* Destroy the player character */
	virtual void Destroyed() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* Returns If the player can reload or not */
	bool CanReload() const;

	/* Returns If the player can fire or not */
	bool CanFire() const;

	/* Returns the current health of the player */
	FORCEINLINE float GetCurrentHealth() const { return CurrentHitPoints; }

	/* Returns the maximum health of the player */
	FORCEINLINE float GetMaxHealth() const { return MaxHitPoints; }

	/* Returns whether the player is alive or not */
	FORCEINLINE bool IsAlive() const { return CurrentHitPoints > 0; }

	/* Returns the player mesh */
	FORCEINLINE USkeletalMeshComponent* GetPlayerMesh() const { return PlayerMesh1P; }

	/* Returns the weapon currently equipped by the player */
	FORCEINLINE class ALoneSurvivorWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

	/* Returns if the player is targeting */
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	bool IsTargeting() const { return bIsTargeting; }

	/* Returns if the Player if firing */
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	bool IsFiring() const { return bIsFiring; }

	/* Returns if the player is running */
	UFUNCTION(BlueprintCallable, Category = "Game|Movement")
	bool IsRunning() const { return bIsRunning; }

	/* Returns the current stance of the player */
	UFUNCTION(BlueprintCallable, Category = "Game|Movement")
	EPlayerStance GetCurrentStance() const { return CurrentStance; }

	/* Returns Targeting speed modifier */
	UFUNCTION(BlueprintCallable, Category = "Game|Movement")
	float GetTargetingSpeedModifier() const { return TargetingSpeedModifier; }

	/* Returns Running speed modifier */
	UFUNCTION(BlueprintCallable, Category = "Game|Movement")
	float GetRunningSpeedModifier() const { return RunningSpeedModifier; }

	/* Returns Crouch speed modifier */
	UFUNCTION(BlueprintCallable, Category = "Game|Movement")
	float GetCrouchSpeedModifier() const { return CrouchSpeedModifier; }

	/* Returns Prone speed modifier */
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

	/* Responsible for collecting the pickups overlapping with the player */
	void CollectPickups();

	/* Triggered when player presses Reload Action Button */
	void OnReload();

	/* Called to stop the reload when the action is interuppted */
	void OnStopReload();

	/* Triggered when player equips the rifle */
	void EquipPrimaryWeapon();

	/* Triggered when player equips the pistol */
	void EquipSecondaryWeapon();

	/* Set new Targeting state */
	inline void SetTargeting(bool bNewTargeting) { bIsTargeting = bNewTargeting; }

	/* Set new running state */
	inline void SetRunning(bool bNewRunning) { bIsRunning = bNewRunning; }

	/* Set new firing state */
	inline void SetFiring(bool bNewFiring) { bIsFiring = bNewFiring; }

	/* Set new Stance */
	inline void SetStance(EPlayerStance NewStance) { CurrentStance = NewStance; }

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

	/* Whether the player is firing */
	uint8 bIsFiring : 1;

	/* Whether player is aiming down the sight */
	uint8 bIsTargeting : 1;

	/* Whether the player is in Prone stance */
	uint8 bIsProne : 1;

	/* Whether the player is in crouch stance */
	uint8 bIsCrouched : 1;

	/* Current Stance of the player i.e. whether the player is in Standing Position, Crouch Position or Prone Position */
	EPlayerStance CurrentStance;

	/* Current Health of the player */
	float CurrentHitPoints;

	// Primary weapon of the player (Rifle)
	class ALoneSurvivorWeapon* PrimaryWeapon;

	// Secondary weapon of the player (Pistol)
	class ALoneSurvivorWeapon* SecondaryWeapon;

	// Current weapon equipped by the player
	class ALoneSurvivorWeapon* CurrentWeapon;

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
