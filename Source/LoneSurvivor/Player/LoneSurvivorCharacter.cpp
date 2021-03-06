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
#include "../Weapons/LoneSurvivorWeapon.h"


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

	//TODO: Spawn Weapons


	EquipPrimaryWeapon();
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

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ALoneSurvivorCharacter::OnStartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ALoneSurvivorCharacter::OnStopFire);

	PlayerInputComponent->BindAction("Target", IE_Pressed, this, &ALoneSurvivorCharacter::OnStartTargeting);
	PlayerInputComponent->BindAction("Target", IE_Released, this, &ALoneSurvivorCharacter::OnStopTargeting);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ALoneSurvivorCharacter::ToggleCrouch);
	
	PlayerInputComponent->BindAction("Prone", IE_Pressed, this, &ALoneSurvivorCharacter::ToggleProne);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ALoneSurvivorCharacter::Interact);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ALoneSurvivorCharacter::OnReload);

	PlayerInputComponent->BindAction("EquipRifle", IE_Pressed, this, &ALoneSurvivorCharacter::EquipPrimaryWeapon);

	PlayerInputComponent->BindAction("EquipHandgun", IE_Pressed, this, &ALoneSurvivorCharacter::EquipSecondaryWeapon);

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
	if (MyController && MyController->IsPlayerInputAllowed())
	{
		bPressedJump = true;
	}
}

void ALoneSurvivorCharacter::OnJumpStop()
{
	ALoneSurvivorPlayerController* MyController = Cast<ALoneSurvivorPlayerController>(Controller);
	if (MyController && MyController->IsPlayerInputAllowed())
	{
		bPressedJump = false;
	}
}

void ALoneSurvivorCharacter::OnStartRunning()
{
	ALoneSurvivorPlayerController* MyController = Cast<ALoneSurvivorPlayerController>(Controller);
	if (MyController && MyController->IsPlayerInputAllowed())
	{
		if (IsTargeting())
		{
			SetTargeting(false);
		}

		//Stop the firing
		OnStopFire();

		//Stop the weapon reload
		OnStopReload();

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
	if (MyController && MyController->IsPlayerInputAllowed())
	{
		bIsFiring = true;
		CurrentWeapon->StartFire();
	}
}

void ALoneSurvivorCharacter::OnStopFire()
{
	bIsFiring = false;
	CurrentWeapon->StopFire();
}

void ALoneSurvivorCharacter::OnStartTargeting()
{
	ALoneSurvivorPlayerController* MyController = Cast<ALoneSurvivorPlayerController>(Controller);
	if (MyController && MyController->IsPlayerInputAllowed())
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
	if (MyController && MyController->IsPlayerInputAllowed())
	{	
		if (bIsCrouched)
		{
			bIsCrouched = false;
			SetStance(EPlayerStance::Standing);
		}
		else
		{
			//TODO: Add Support for sliding (Crouch while running)
			bIsCrouched = true;
			SetStance(EPlayerStance::Crouch);
		}
	}
}

void ALoneSurvivorCharacter::ToggleProne()
{
	ALoneSurvivorPlayerController* MyController = Cast<ALoneSurvivorPlayerController>(Controller);
	if (MyController && MyController->IsPlayerInputAllowed())
	{
		if (bIsProne)
		{
			bIsProne = false;
			SetStance(EPlayerStance::Standing);
		}
		else
		{
			//Add logic to change the eye height
			bIsProne = true;
			SetStance(EPlayerStance::Prone);
		}
	}
}

void ALoneSurvivorCharacter::MoveForward(const float Value)
{
	if (Controller && Value != 0.f)
	{
		FRotator rotation = Controller->GetControlRotation();
		FVector direction = FRotationMatrix(rotation).GetUnitAxis(EAxis::X);
		AddMovementInput(direction, Value);
	}
}

void ALoneSurvivorCharacter::MoveRight(const float Value)
{
	if (Controller && Value != 0.f)
	{
		FQuat rotation = GetActorQuat();
		FVector direction = FQuatRotationMatrix(rotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(direction, Value);
	}
}

void ALoneSurvivorCharacter::TurnAtRate(const float Value)
{
	AddControllerYawInput(BaseTurnRate * Value * GetWorld()->GetDeltaSeconds());
}

void ALoneSurvivorCharacter::Interact()
{
	ALoneSurvivorPlayerController *MyController = Cast<ALoneSurvivorPlayerController>(Controller);
	if (MyController && MyController->IsPlayerInputAllowed())
	{
		//Collect Pickups
		CollectPickups();

		// Add other things later - Other game objects
	}
}

//TODO: Modify this method to add the pickups to the inventory once the inventory is in place
void ALoneSurvivorCharacter::CollectPickups()
{
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
					//Add ammo to the weapon if the ammo is needed
					EAmmoType AmmoType = AmmoPickup->GetAmmoType();
					if (AmmoType == EAmmoType::Handgun && SecondaryWeapon->GetCurrentAmmo() < SecondaryWeapon->GetMaxAmmo())
					{
						//Add to the secondary weapon
						SecondaryWeapon->AddAmmo(AmmoPickup->GetAmmoCapacity());
					}
					else if (AmmoType == EAmmoType::Rifle && PrimaryWeapon->GetCurrentAmmo() < PrimaryWeapon->GetMaxAmmo())
					{
						//Add to the primary weapon
						PrimaryWeapon->AddAmmo(AmmoPickup->GetAmmoCapacity());
					}
				}
				else
				{
					AHealthPickup* HealthPickup = Cast<AHealthPickup>(PickupObject);
					if (HealthPickup)
					{
						//Add the health
						if (CurrentHitPoints < MaxHitPoints)
						{
							CurrentHitPoints += HealthPickup->GetHealingCapacity();
							if (CurrentHitPoints >= MaxHitPoints)
								CurrentHitPoints = MaxHitPoints;
						}
					}
				}

				//Destroy the Pickup Object
				PickupObject->OnPickupCollection_Implementation();
			}
		}
	}
}

//Reload the weapon
void ALoneSurvivorCharacter::OnReload()
{
	ALoneSurvivorPlayerController *MyController = Cast<ALoneSurvivorPlayerController>(Controller);
	if (MyController && MyController->IsPlayerInputAllowed())
	{
		if (CurrentWeapon)
		{
			CurrentWeapon->StartReload();
		}
	}
}

void ALoneSurvivorCharacter::OnStopReload()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopReload();
	}
}

void ALoneSurvivorCharacter::EquipPrimaryWeapon()
{
	ALoneSurvivorPlayerController* MyController = Cast<ALoneSurvivorPlayerController>(Controller);
	if (MyController)
	{
		if (CurrentWeapon != PrimaryWeapon && PrimaryWeapon != nullptr)
		{
			if (CurrentWeapon)
				CurrentWeapon->OnUnEquip();

			CurrentWeapon = PrimaryWeapon;

			CurrentWeapon->SetWeaponOwner(this);
			CurrentWeapon->OnEquip();
		}
	}
}

void ALoneSurvivorCharacter::EquipSecondaryWeapon()
{
	ALoneSurvivorPlayerController* MyController = Cast<ALoneSurvivorPlayerController>(Controller);
	if (MyController)
	{
		if (CurrentWeapon != SecondaryWeapon)
		{
			if (CurrentWeapon)
				CurrentWeapon->OnUnEquip();

			CurrentWeapon = SecondaryWeapon;

			CurrentWeapon->SetWeaponOwner(this);
			CurrentWeapon->OnEquip();
		}
	}
}

bool ALoneSurvivorCharacter::CanReload() const
{
	ALoneSurvivorPlayerController* MyController = Cast<ALoneSurvivorPlayerController>(Controller);
	if (MyController && MyController->IsPlayerInputAllowed())
	{
		return IsAlive();
	}

	return false;
}

bool ALoneSurvivorCharacter::CanFire() const
{
	ALoneSurvivorPlayerController* MyController = Cast<ALoneSurvivorPlayerController>(Controller);
	if (MyController && MyController->IsPlayerInputAllowed())
	{
		return IsAlive();
	}

	return false;
}