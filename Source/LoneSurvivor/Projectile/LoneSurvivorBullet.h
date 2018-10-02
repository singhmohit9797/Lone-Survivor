// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/LoneSurvivorProjectile.h"
#include "LoneSurvivorBullet.generated.h"

UENUM()
enum class EBulletType : uint8
{
	Rifle45mm,
	HandgunBullet9mm
};

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
	/* Bullet Type represented by this class */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
	EBulletType BulletType;

private:
	/* Delegate to handle the hit event of the projectile */
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherHitComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
