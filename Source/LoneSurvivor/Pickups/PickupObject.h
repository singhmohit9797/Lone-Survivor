// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyObject.h"
#include "PickupObject.generated.h"

/**
 * 
 */
UCLASS()
class LONESURVIVOR_API APickupObject
	: public AMyObject
{
	GENERATED_BODY()

public:
	//Initialise the Pickup object
	APickupObject();

	/* Returns the state of the pickup */
	UFUNCTION(BlueprintPure, Category = "Pickup State")
	FORCEINLINE uint8 GetActive() const { return bIsActive; }

	/* Returns the name of the Pickup Object */
	UFUNCTION(BlueprintPure, Category = "Attributes")
	FORCEINLINE FName GetObjectName() const { return PickupObjectName; }

	/* Deactivates the pickup object */
	UFUNCTION(BlueprintCallable, Category = "Pickup State")
	void DeactivatePickup();

	UFUNCTION(BlueprintNativeEvent, Category = "Pickup Collection")
	void OnPickupCollection();
	virtual void OnPickupCollection_Implementation();

private:
	/* Function delegate to handle the begin overlap events of the pickup object */
	UFUNCTION()
	virtual void OnBeginOverlap(class UPrimitiveComponent* OverlapComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/* Function delegate to handle the end overlap events of the pickup object */
	UFUNCTION()
	virtual void OnEndOverlap(class UPrimitiveComponent* OverlapComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	/* Whether the pickup object is active */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup State")
	uint8 bIsActive : 1;

	/* Name of the Pickup Object */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FName PickupObjectName;

	/* Reference to the widget class that will show the object name when use draws near the object */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UPickupObjectWidget> PickupWidgetClass;

	/* The widget that will be displayed on the screen */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	class UPickupObjectWidget* PickupWidget;

private:
	/* The sphere trigger component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", META = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionSphere;
};
