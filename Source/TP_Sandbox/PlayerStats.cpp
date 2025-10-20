// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStats.h"

// Sets default values for this component's properties
UPlayerStats::UPlayerStats()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UPlayerStats::DecreaseHealth(float Amount)
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

void UPlayerStats::IncreaseHealth(float Amount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0, MaxHealth);
	OnHealthUpdated.Broadcast(CurrentHealth, MaxHealth);
}

void UPlayerStats::IncreaseMaxHealth(float Amount)
{
	MaxHealth += Amount;
	CurrentHealth = MaxHealth;
	OnHealthUpdated.Broadcast(CurrentHealth, MaxHealth);
}

bool UPlayerStats::DecreaseStamina(float Amount)
{
	if (CurrentStamina - Amount < 0)
		return false;

	float previousStamina = CurrentStamina;
	CurrentStamina -= Amount;
	OnStaminaUpdated.Broadcast(CurrentStamina, previousStamina, MaxStamina);
	return true;
}

void UPlayerStats::IncreaseStamina(float Amount)
{
	float PreviousStamina = CurrentStamina;
	CurrentStamina = FMath::Clamp(CurrentStamina + Amount, 0, MaxStamina);
	OnStaminaUpdated.Broadcast(CurrentStamina, PreviousStamina, MaxStamina);
}

void UPlayerStats::IncreaseMaxStamina(float Amount)
{
	MaxStamina += Amount;
}

void UPlayerStats::IncreaseXp(float Amount)
{
	CurrentXp += Amount;
	while (CurrentXp >= MaxXp)
	{
		CurrentXp -= MaxXp;
		MaxXp += MaxXp * 0.1f;
		CurrentLevel++;
		OnLevelUpdate.Broadcast(CurrentLevel);
	}
	
	OnXpUpdated.Broadcast(CurrentXp, MaxXp);
}

// Called when the game starts
void UPlayerStats::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CurrentHealth = MaxHealth;
	CurrentStamina = MaxStamina;
}


// Called every frame
void UPlayerStats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
