// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventorySystem/Item.h"
#include "InventorySystem/Slot.h"
#include "EquipmentSystem.generated.h"


class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TP_SANDBOX_API UEquipmentSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentSystem();

	void TryPickupItem();
public:
	UFUNCTION(BlueprintCallable)
	void AddItem(const FItem& Item);
	
	UFUNCTION(BlueprintCallable)
	void AddItemFromSlot(const FSlot& Slot);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TObjectPtr<UCameraComponent> FollowCamera;
	
	TArray<FItem> Items;
		
};
