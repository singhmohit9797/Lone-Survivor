// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/LoneSurvivorProjectile.h"
#include "LoneSurvivorBullet.generated.h"

/**
 * 
 */
UCLASS()
class LONESURVIVOR_API ALoneSurvivorBullet : public ALoneSurvivorProjectile
{
	GENERATED_BODY()
	
public:
	ALoneSurvivorBullet();
	
protected:

private:
	/* Delegate to handle the hit event of the projectile */
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherHitComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
