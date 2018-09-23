// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupObject.h"
#include "Engine.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

#include "../Player/LoneSurvivorCharacter.h"
#include "../Player/LoneSurvivorPlayerController.h"
#include "../UI/PickupObjectWidget.h"

APickupObject::APickupObject()
	: AMyObject()
{
	//Set as active by default
	bIsActive = true;

	GetObjectMesh()->SetSimulatePhysics(false);

	//Create the trigger component
	CollisionSphere = CreateDefaultSubobject<class USphereComponent>(TEXT("Collision Sphere"));
	CollisionSphere->SetGenerateOverlapEvents(true);
	CollisionSphere->SetupAttachment(RootComponent);
	//CollisionSphere->SetCollisionProfileName("CP_LoneSurvivorPickup"); // Uncomment this after creating the profile in the editor;
	CollisionSphere->InitSphereRadius(50.f);
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APickupObject::OnBeginOverlap);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &APickupObject::OnEndOverlap);
}

//Deactivate the pickup
void APickupObject::DeactivatePickup()
{
	bIsActive = false;
}

void APickupObject::OnPickupCollection_Implementation()
{
	//Display the debug message on the screen
	const FString DebugMessage = TEXT("Picked up ") + GetName();
	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Cyan, DebugMessage, true, FVector2D::UnitVector * 1.5);

	//Destroy the Widget
	if (PickupWidget)
	{
		// Remove the object name widget from the viewport
		if(PickupWidget->IsInViewport())
			PickupWidget->RemoveFromParent();
		
		PickupWidget->Destruct();
	}

	//Mark the object to be destroyed in the next frame
	Destroy();
}

void APickupObject::OnBeginOverlap(class UPrimitiveComponent* OverlapComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Cast to the lone survivor character and proceed if the cast succeeds
	ALoneSurvivorCharacter* Player = Cast<ALoneSurvivorCharacter>(OtherActor);
	if (Player)
	{
		//Create the widget if the widget is not created
		if (!PickupWidget)
		{
			if (!PickupWidgetClass)
			{
				ALoneSurvivorPlayerController* PlayerController = Cast<ALoneSurvivorPlayerController>(Player->GetController());
				if (PlayerController)
				{
					PickupWidget = CreateWidget<UPickupObjectWidget>(PlayerController, PickupWidgetClass);
					UTextBlock* PickupName = PickupWidget->WidgetTree->FindWidget<UTextBlock>(FName(TEXT("PickupName")));
					PickupName->SetCursor(EMouseCursor::None);
					PickupName->SetText(FText::FromName(PickupObjectName));
					PickupName->SetFont(FSlateFontInfo(PickupName->GetDefaultFontName(), 146));
				}
			}
		}

		//Show the object name widget
		PickupWidget->AddToViewport(99);
	}
}

void APickupObject::OnEndOverlap(class UPrimitiveComponent* OverlapComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ALoneSurvivorCharacter* Player = Cast<ALoneSurvivorCharacter>(OtherActor);
	if (Player)
	{
		//remove the object name widget if the widget is created and is in viewport
		if (!PickupWidget && PickupWidget->IsInViewport())
		{
			PickupWidget->RemoveFromParent();
		}
	}
}