// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CurrentHealth = MaxHealth;
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]
	{
		OnHealthUpdated.Broadcast(CurrentHealth, MaxHealth);
	});
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UHealthComponent::DecreaseHealth(float Amount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Amount, 0, MaxHealth);
	OnHealthUpdated.Broadcast(CurrentHealth, MaxHealth);
	if (IsDead())
	{
		OnDeath.Broadcast();
		return true;
	}

	return false;
}

void UHealthComponent::IncreaseHealth(float Amount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0, MaxHealth);
	OnHealthUpdated.Broadcast(CurrentHealth, MaxHealth);
}

void UHealthComponent::IncreaseMaxHealth(float Amount)
{
	MaxHealth += Amount;
	CurrentHealth = MaxHealth;
	OnHealthUpdated.Broadcast(CurrentHealth, MaxHealth);
}
