// Fill out your copyright notice in the Description page of Project Settings.

#include "LoneSurvivorCharacter.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine.h"

#include "LoneSurvivorPlayerController.h"
#include "LoneSurvivorCharacterMovement.h"
#include "../Pickups/PickupObject.h"
#include "../Pickups/AmmoPickup.h"
#include "../Pickups/HealthPickup.h"


// Sets default values
ALoneSurvivorCharacter::ALoneSurvivorCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<class ULoneSurvivorCharacterMovement>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.f);

	/************************ Setup the camera component *************************/
	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FPSCamera->SetupAttachment(GetCapsuleComponent());
	FPSCamera->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FPSCamera->bUsePawnControlRotation = true;

	/************************ Setup the Player Mesh Component ***********************/
	PlayerMesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Player Mesh"));
	PlayerMesh1P->SetupAttachment(FPSCamera);
	PlayerMesh1P->bOnlyOwnerSee = true;
	PlayerMesh1P->bOwnerNoSee = false;
	PlayerMesh1P->bCastDynamicShadow = false;
	PlayerMesh1P->SetReceivesDecals(false);
	PlayerMesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	PlayerMesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
	PlayerMesh1P->SetGenerateOverlapEvents(true);
	//PlayerMesh1P->SetCollisionProfileName("CP_LoneSurvivorCharacter");  -- Uncomment this line after creating the collision profile in the editor

	GetMesh()->bOwnerNoSee = true;
	GetMesh()->bOwnerNoSee = true;
	GetMesh()->bReceivesDecals = false;

	// Initialise the player attributes
	MaxHitPoints = 2000.f;
	bIsTargeting = false;
	bIsFiring = false;
	bIsJumping = false;
	bIsRunning = false;
	bIsCrouched = false;
	bIsProne = false;

	//Setup initial variables
	TargetingSpeedModifier = CrouchSpeedModifier = 0.75f;
	ProneSpeedModifier = 0.25f;
	RunningSpeedModifier = 1.5f;
	
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
}

// Setup Initial variables
void ALoneSurvivorCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//Set the crouch speed
	GetCharacterMovement()->MaxWalkSpeedCrouched *= CrouchSpeedModifier;

	//Set the intial variables
	CurrentHitPoints = MaxHitPoints;
	CurrentStance = EPlayerStance::Standing;
}

//Destroy the character object
void ALoneSurvivorCharacter::Destroyed()
{
	Super::Destroyed();
}

// Called when the game starts or when spawned
void ALoneSurvivorCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALoneSurvivorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALoneSurvivorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	/******************** Actions Bindings *********************/
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ALoneSurvivorCharacter::OnJumpStart);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ALoneSurvivorCharacter::OnJumpStop);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ALoneSurvivorCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ALoneSurvivorCharacter::OnStopRunning);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ALoneSurvivorCharacter::ToggleCrouch);
	PlayerInputComponent->BindAction("Prone", IE_Pressed, this, &ALoneSurvivorCharacter::ToggleProne);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ALoneSurvivorCharacter::OnStartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ALoneSurvivorCharacter::OnStopFire);

	PlayerInputComponent->BindAction("Target", IE_Pressed, this, &ALoneSurvivorCharacter::OnStartTargeting);
	PlayerInputComponent->BindAction("Target", IE_Released, this, &ALoneSurvivorCharacter::OnStopTargeting);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ALoneSurvivorCharacter::Interact);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ALoneSurvivorCharacter::OnReload);

	/******************** Axis Bindings *********************/
	PlayerInputComponent->BindAxis("MoveForward", this, &ALoneSurvivorCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALoneSurvivorCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnAtRate", this, &ALoneSurvivorCharacter::TurnAtRate);

	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void ALoneSurvivorCharacter::OnJumpStart()
{
	ALoneSurvivorPlayerController* MyController = Cast<ALoneSurvivorPlayerController>(Controller);
	if (MyController && MyController->IsGameInputAllowed())
	{
		bPressedJump = true;
	}
}

void ALoneSurvivorCharacter::OnJumpStop()
{
	ALoneSurvivorPlayerController* MyController = Cast<ALoneSurvivorPlayerController>(Controller);
	if (MyController && MyController->IsGameInputAllowed())
	{
		bPressedJump = false;
	}
}

void ALoneSurvivorCharacter::OnStartRunning()
{
	ALoneSurvivorPlayerController* MyController = Cast<ALoneSurvivorPlayerController>(Controller);
	if (MyController && MyController->IsGameInputAllowed())
	{
		if (IsTargeting())
		{
			SetTargeting(false);
		}

		OnStopFire();
		bIsRunning = true;
	}
}

void ALoneSurvivorCharacter::OnStopRunning()
{
	bIsRunning = false;
}

void ALoneSurvivorCharacter::OnStartFire()
{
	ALoneSurvivorPlayerController *MyController = Cast<ALoneSurvivorPlayerController>(Controller);
	if (MyController && MyController->IsGameInputAllowed())
	{
		bIsFiring = true;
		//StartWeaponFire();
	}
}

void ALoneSurvivorCharacter::OnStopFire()
{
	bIsFiring = false;
	//StopWeaponFire();
}

void ALoneSurvivorCharacter::OnStartTargeting()
{
	ALoneSurvivorPlayerController* MyController = Cast<ALoneSurvivorPlayerController>(Controller);
	if (MyController && MyController->IsGameInputAllowed())
	{
		if (IsRunning())
		{
			SetRunning(false);
		}
		SetTargeting(true);
	}

}

void ALoneSurvivorCharacter::OnStopTargeting()
{
	SetTargeting(false);
}

void ALoneSurvivorCharacter::ToggleCrouch()
{
	ALoneSurvivorPlayerController* MyController = Cast<ALoneSurvivorPlayerController>(Controller);
	if (MyController && MyController->IsGameInputAllowed())
	{
		
		if (bIsCrouched)
		{
			const FString msg1 = "Stood";
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, msg1, true, FVector2D::UnitVector * 1.5f);
			bIsCrouched = false;
			SetStance(EPlayerStance::Standing);
		}
		else
		{
			//TODO: Add Support for sliding (Crouch while running)
			const FString msg1 = "Crouched";
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, msg1, true, FVector2D::UnitVector * 1.5f);
			bIsCrouched = true;
			SetStance(EPlayerStance::Crouch);
		}
		/*FVector RootComponentLocation = RootComponent->GetComponentLocation();
		if (bIsCrouched)
		{
			SetStance(EPlayerStance::Standing);
			RootComponentLocation -= FVector(0, CrouchedEyeHeight, 0);
		}
		else
		{
			SetStance(EPlayerStance::Crouch);
		}
		RootComponent->SetRelativeLocation(RootComponentLocation);*/
	}
}

void ALoneSurvivorCharacter::ToggleProne()
{
	ALoneSurvivorPlayerController* MyController = Cast<ALoneSurvivorPlayerController>(Controller);
	if (MyController && MyController->IsGameInputAllowed())
	{
		if (bIsProne)
		{
			bIsProne = false;
			CurrentStance = EPlayerStance::Standing;
		}
		else
		{
			//Add logic to change the eye height
			bIsProne = true;
			CurrentStance = EPlayerStance::Prone;
		}
	}
}

// Move forward or backward
void ALoneSurvivorCharacter::MoveForward(const float Value)
{
	if (Controller && Value != 0.f)
	{
		FRotator rotation = Controller->GetControlRotation();
		FVector direction = FRotationMatrix(rotation).GetUnitAxis(EAxis::X);
		AddMovementInput(direction, Value);
	}
}

// Move Left or Right
void ALoneSurvivorCharacter::MoveRight(const float Value)
{
	if (Value != 0.f)
	{
		FQuat rotation = GetActorQuat();
		FVector direction = FQuatRotationMatrix(rotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(direction, Value);
	}
}

// Turn the camera
void ALoneSurvivorCharacter::TurnAtRate(const float Value)
{
	AddControllerYawInput(BaseTurnRate * Value * GetWorld()->GetDeltaSeconds());
}

//TODO: Modify this method to add the pickups to the inventory once the inventory is in place
void ALoneSurvivorCharacter::Interact()
{
	ALoneSurvivorPlayerController *MyController = Cast<ALoneSurvivorPlayerController>(Controller);
	if (MyController && MyController->IsGameInputAllowed())
	{
		//Collecto Pickups
		TArray<AActor*> OverlappingActors;
		GetOverlappingActors(OverlappingActors, APickupObject::StaticClass());

		if (OverlappingActors.Num() > 0)
		{
			for (int32 index = 0; index < OverlappingActors.Num(); index++)
			{
				APickupObject* PickupObject = Cast<APickupObject>(OverlappingActors[index]);
				if (PickupObject)
				{
					//Check if the pick is the ammo pickup
					AAmmoPickup* AmmoPickup = Cast<AAmmoPickup>(PickupObject);
					if (AmmoPickup)
					{
						//Add ammo to the weapon
						EAmmoType::AmmoType AmmoType = AmmoPickup->GetAmmoType();
						if (AmmoType == EAmmoType::Bullet)
						{
							//Add to the secondary weapon
						}
						else if (AmmoType == EAmmoType::Rifle)
						{
							//Add to the primary weapon
						}
					}
					else
					{
						AHealthPickup* HealthPickup = Cast<AHealthPickup>(PickupObject);
						if (HealthPickup)
						{
							//Add the health
							CurrentHitPoints += HealthPickup->GetHealingCapacity();
							if (CurrentHitPoints >= MaxHitPoints)
								CurrentHitPoints = MaxHitPoints;
						}
					}

					//Destroy the Pickup Object
					PickupObject->OnPickupCollection_Implementation();
				}
			}
		}
	}
}

//Reload the weapon
void ALoneSurvivorCharacter::OnReload()
{
	ALoneSurvivorPlayerController *MyController = Cast<ALoneSurvivorPlayerController>(Controller);
	if (MyController && MyController->IsGameInputAllowed())
	{

	}
}

//Set new Targeting state
void ALoneSurvivorCharacter::SetTargeting(bool bNewTargeting)
{
	bIsTargeting = bNewTargeting;
}

// Set new Running state
void ALoneSurvivorCharacter::SetRunning(bool bNewRunning)
{
	bIsRunning = bNewRunning;
}

//Set new Firing state
void ALoneSurvivorCharacter::SetFiring(bool bNewFiring)
{
	bIsFiring = bNewFiring;
}

//Set new Stance
void ALoneSurvivorCharacter::SetStance(EPlayerStance::Stance bNewStance)
{
	CurrentStance = bNewStance;
}

bool ALoneSurvivorCharacter::CanReload() const
{
	ALoneSurvivorPlayerController* MyController = Cast<ALoneSurvivorPlayerController>(Controller);
	if (MyController && MyController->IsGameInputAllowed())
	{
		return IsAlive();
	}

	return false;
}

bool ALoneSurvivorCharacter::CanFire() const
{
	ALoneSurvivorPlayerController* MyController = Cast<ALoneSurvivorPlayerController>(Controller);
	if (MyController && MyController->IsGameInputAllowed())
	{
		return IsAlive();
	}

	return false;
}