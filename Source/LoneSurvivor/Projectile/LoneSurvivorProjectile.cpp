// Fill out your copyright notice in the Description page of Project Settings.

#include "LoneSurvivorProjectile.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ALoneSurvivorProjectile::ALoneSurvivorProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<class UCapsuleComponent>(TEXT("ProjectileCollisionComponent"));
	CollisionComponent->InitCapsuleSize(0.3f, 0.5f);
	//CollisionComponent->SetCollisionProfileName("CP_LoneSurvivorProjectile");

	ProjectileMovementComponent = CreateDefaultSubobject<class UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 3600.f;
	ProjectileMovementComponent->MaxSpeed = 3600.f;
	ProjectileMovementComponent->bShouldBounce = false;

	OnHitDamage = 100.f;
	CollisionComponent->OnComponentHit.AddDynamic(this, &ALoneSurvivorProjectile::OnHit);
}

// Called when the game starts or when spawned
void ALoneSurvivorProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALoneSurvivorProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALoneSurvivorProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherHitComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
}