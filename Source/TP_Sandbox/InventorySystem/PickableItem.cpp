// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/PickableItem.h"

#include "Item.h"

// Sets default values
APickableItem::APickableItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
	Mesh->SetCollisionProfileName(TEXT("BlockAll"));
	Mesh->SetSimulatePhysics(false);
	RootComponent = Mesh;
}

void APickableItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	static const FString Context("PickableItem");
	if (const FItem* Items = Slot.Item.GetRow<FItem>(Context))
	{
		Mesh->SetStaticMesh(Items->Mesh);
	}
}

// Called when the game starts or when spawned
void APickableItem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APickableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
