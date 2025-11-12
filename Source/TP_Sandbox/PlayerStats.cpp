// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStats.h"

#include "GeometryCollection/GeometryCollectionParticlesData.h"

// Sets default values for this component's properties
UPlayerStats::UPlayerStats()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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
		OnLevelUpdate.Broadcast(CurrentLevel, true);
	}
	
	OnXpUpdated.Broadcast(CurrentXp, MaxXp);
}

// Called when the game starts
void UPlayerStats::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CurrentStamina = MaxStamina;
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]
	{
		OnStaminaUpdated.Broadcast(CurrentStamina, CurrentStamina, MaxStamina);
		OnXpUpdated.Broadcast(CurrentXp, MaxXp);
		OnLevelUpdate.Broadcast(CurrentLevel, false);
	});
}


// Called every frame
void UPlayerStats::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
