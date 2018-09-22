// Fill out your copyright notice in the Description page of Project Settings.

#include "LoneSurvivorWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"

#include "../Player/LoneSurvivorCharacter.h"
#include "../Player/LoneSurvivorPlayerController.h"
#include "../Pickups/PickupObject.h"

// Sets default values
ALoneSurvivorWeapon::ALoneSurvivorWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*******************  Create the weapon mesh ****************/
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	WeaponMesh->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::OnlyTickPoseWhenRendered;
	WeaponMesh->bReceivesDecals = false;
	WeaponMesh->CastShadow = false;
	RootComponent = WeaponMesh;

	/******************* Create the muzzle location object ********/
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Mzuzle Offset"));
	MuzzleLocation->SetupAttachment(WeaponMesh);

	// Initialize the properties
	bWantsToReload = false;
	bWantsToFire = false;
	CurrentWeaponState = EWeaponState::Idle;
	AmmoPerMag = 30;
	InitialMags = 8;
	MaxAmmo = 300;
}

// Called when the game starts or when spawned
void ALoneSurvivorWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALoneSurvivorWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALoneSurvivorWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//Set the relative location
	MuzzleLocation->SetRelativeLocation(MuzzleOffset);

	// Initialize the current ammo
	CurrentMagAmmo = AmmoPerMag;
	CurrentAmmo = InitialMags * AmmoPerMag;

	if (CurrentAmmo > MaxAmmo)
		CurrentAmmo = MaxAmmo;
}

bool ALoneSurvivorWeapon::CanFire() const
{
	bool bCanFire = WeaponOwner && WeaponOwner->CanFire();
	bool bIsInValidStateToFire = (CurrentWeaponState == EWeaponState::Idle) || (CurrentWeaponState == EWeaponState::Firing);
	return bCanFire && bIsInValidStateToFire;
}

bool ALoneSurvivorWeapon::CanReload() const
{
	bool bCanReload = WeaponOwner && WeaponOwner->CanReload();
	bool bIsInValidStateToReload = (CurrentWeaponState == EWeaponState::Idle || CurrentWeaponState == EWeaponState::Firing);
	bool bIsReloadNeeded = (CurrentMagAmmo >= 0 && CurrentMagAmmo < AmmoPerMag) && (CurrentAmmo > 0);
	return bCanReload && bIsInValidStateToReload && bIsReloadNeeded;
}

void ALoneSurvivorWeapon::StartFire()
{
	if (!bWantsToFire)
	{
		bWantsToFire = true;
		
		if (bWantsToReload)
		{
			if (CanReload())
				SetWeaponState(EWeaponState::Reloading);
			else
				SetWeaponState(CurrentWeaponState);
		}
		else if (!bWantsToReload && CanFire())
		{
			SetWeaponState(EWeaponState::Firing);
		}
	}
}

void ALoneSurvivorWeapon::StopFire()
{
	if (bWantsToFire)
	{
		bWantsToFire = false;
		SetWeaponState(EWeaponState::Idle);
	}
}

void ALoneSurvivorWeapon::StartReload()
{
	if (!bWantsToReload)
	{
		bWantsToReload = true;
		if (CanReload())
		{
			SetWeaponState(EWeaponState::Reloading);

			//Start Animation before the magazine is reloaded

			StartWeaponReload();
		}
		else
		{
			//Notify can't reload
			SetWeaponState(CurrentWeaponState);
		}
	}
}

void ALoneSurvivorWeapon::StopReload()
{
	if (bWantsToReload)
	{
		bWantsToReload = false;

		if (bWantsToFire && CanFire())
			SetWeaponState(EWeaponState::Firing);
		else
			SetWeaponState(EWeaponState::Idle);
	}
}

void ALoneSurvivorWeapon::SetWeaponOwner(class ALoneSurvivorCharacter* NewOwner)
{
	if (NewOwner)
	{
		Instigator = NewOwner;
		WeaponOwner = NewOwner;
	}
}

void ALoneSurvivorWeapon::SetWeaponState(EWeaponState::Weapon_State NewState)
{
	EWeaponState::Weapon_State PrevState = CurrentWeaponState;

	CurrentWeaponState = NewState;

	if (PrevState == EWeaponState::Firing && (NewState == EWeaponState::Idle || NewState == EWeaponState::Reloading))
		StopWeaponFire();

	else if ((PrevState == EWeaponState::Idle || PrevState == EWeaponState::Reloading) && NewState == EWeaponState::Firing)
		StartWeaponFire();
}

void ALoneSurvivorWeapon::StartWeaponFire()
{
	//Check if can fire weapon (Have bullets)
	if (CurrentMagAmmo > 0)
	{
		//Fire Weapon
			//Get the direction
		FRotator SpawnRotation;
		FVector SpawnLocation;

		//Get the location where to spawn the bullet
		ALoneSurvivorPlayerController* MyController = Cast<ALoneSurvivorPlayerController>(WeaponOwner->GetController());
		if (MyController)
		{
			MyController->GetActorEyesViewPoint(SpawnLocation, SpawnRotation);
		}
		else
		{
			SpawnRotation = WeaponOwner->GetControlRotation();
			SpawnLocation = MuzzleLocation->GetComponentLocation() + SpawnRotation.RotateVector(MuzzleOffset);
		}
		//TODO: Add spread to the bullet based on the current action of the player
		
		//Consume Ammo
		CurrentMagAmmo--;
	}
	//If can not fire, check if can reload
	else if (CanReload())
	{
		//Start Reload
		SetWeaponState(EWeaponState::Reloading);
		StartReload();
	}
	//Else Set State to IDLE
	else
	{
		//Notify can't fire
		SetWeaponState(EWeaponState::Idle);
	}

}

void ALoneSurvivorWeapon::StopWeaponFire()
{
	//Stop the fire animations and sounds
}

void ALoneSurvivorWeapon::StartWeaponReload()
{
	//Check if the reload is even needed
	if (CurrentMagAmmo < AmmoPerMag)
	{
		//Check if the reload is possible
		if (CurrentAmmo > 0)
		{
			int32 AmountNeededToRefill = AmmoPerMag - CurrentMagAmmo;
			int32 AmountRefilled = (CurrentAmmo >= AmountNeededToRefill) ? AmountNeededToRefill : (AmountNeededToRefill - CurrentAmmo);
			CurrentMagAmmo += AmountRefilled;
			CurrentAmmo -= AmountRefilled;
		}
	}
}