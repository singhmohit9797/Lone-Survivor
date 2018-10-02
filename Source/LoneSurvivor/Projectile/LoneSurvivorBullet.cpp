// Fill out your copyright notice in the Description page of Project Settings.

#include "LoneSurvivorBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"

ALoneSurvivorBullet::ALoneSurvivorBullet()
{
	OnHitDamage = 250.f;

	GetMovementComponent()->InitialSpeed = 90000.0f;
	GetMovementComponent()->MaxSpeed = 90000.0f;

	BulletType = EBulletType::Rifle45mm;
}

void ALoneSurvivorBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherHitComp, FVector NormalImpulse, const FHitResult& Hit)
{

}