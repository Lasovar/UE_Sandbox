// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/EquipmentSystem.h"

#include "TP_SandboxCharacter.h"
#include "Camera/CameraComponent.h"
#include "InventorySystem/PickableItem.h"

// Sets default values for this component's properties
UEquipmentSystem::UEquipmentSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UEquipmentSystem::TryPickupItem()
{
	TArray<FHitResult> Hits;

	const FName DebugName("Interact");
	const FCollisionShape Sphere = FCollisionShape::MakeSphere(15.f);
	const FVector CameraLocation = FollowCamera->GetComponentLocation();
	
	FCollisionQueryParams CollisionParams = FCollisionQueryParams(DebugName, true);
	CollisionParams.AddIgnoredActor(GetOwner());
	GetWorld()->DebugDrawTraceTag = DebugName;
	
	if (GetWorld()->SweepMultiByChannel(
		Hits,
		CameraLocation,
		CameraLocation + FollowCamera->GetForwardVector() * 1000.0f,
		FQuat::Identity,
		ECC_Visibility,
		Sphere,
		CollisionParams
	))
	{
		for (const FHitResult& Hit : Hits)
		{
			if (!Hit.GetActor()->ActorHasTag("Interactable"))
				continue;

			if (const TObjectPtr<APickableItem> Pickable = Cast<APickableItem>(Hit.GetActor()))
			{
				AddItemFromSlot(Pickable->Slot);
				Pickable->Destroy();
			}
		}
	}
}


// Called when the game starts
void UEquipmentSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (TObjectPtr<ATP_SandboxCharacter> Character = Cast<ATP_SandboxCharacter>(GetOwner()))
	{
		FollowCamera = Character->GetFollowCamera();
	}
}


// Called every frame
void UEquipmentSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipmentSystem::AddItem(const FItem& Item)
{
	Items.Add(Item);
}

void UEquipmentSystem::AddItemFromSlot(const FSlot& Slot)
{
	static const FString Context("EquipmentSystem::AddItemFromSlot");
	if (const FItem* Item = Slot.Item.GetRow<FItem>(Context))
	{
		AddItem(*Item);
	}
}



