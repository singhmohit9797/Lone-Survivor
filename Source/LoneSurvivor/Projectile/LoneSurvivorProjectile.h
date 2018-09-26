// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LoneSurvivorProjectile.generated.h"

UCLASS()
class LONESURVIVOR_API ALoneSurvivorProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALoneSurvivorProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Delegate to handle the hit event of the projectile */
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherHitComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Returns the collision component of the projectile */
	FORCEINLINE class UCapsuleComponent* GetCollisionComponent() const { return CollisionComponent; }

	/* Returns the movement component of the projectile */
	FORCEINLINE class UProjectileMovementComponent* GetMovementComponent() const { return ProjectileMovementComponent; }

	/* Returns the damage dealt by the projectile on hit */
	FORCEINLINE float GetHitDamge() const { return OnHitDamage; }

protected:
	/* Damage dealt by the projectile on collision */
	float OnHitDamage;

private:
	/* Collision component of the projectile */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", META = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CollisionComponent;

	/* Movement component of the projectile */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", META = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovementComponent;
	
};
