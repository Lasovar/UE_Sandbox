// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.h"
#include "PlayerStats.generated.h"

class UHealthComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStaminaUpdated, float, currentStamina, float, previousStamina, float,
                                               maxStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnXpUpdated, float, currentXp, float, maxXp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevelUpdate, int32, currentLevel, bool, playEffects);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TP_SANDBOX_API UPlayerStats : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStats();

	float GetCurrentStamina() const { return CurrentStamina; }
public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStaminaUpdated OnStaminaUpdated;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnLevelUpdate OnLevelUpdate;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnXpUpdated OnXpUpdated;

	UFUNCTION(BlueprintCallable, Category="Player Stats")
	virtual bool DecreaseStamina(float Amount);
	UFUNCTION(BlueprintCallable, Category="Player Stats")
	virtual void IncreaseStamina(float Amount);
	UFUNCTION(BlueprintCallable, Category="Player Stats")
	virtual void IncreaseMaxStamina(float Amount);
	
	UFUNCTION(BlueprintCallable, Category="Player Stats")
	virtual void IncreaseXp(float Amount);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Stats")
	float MaxStamina = 100.0f;
	UPROPERTY(BlueprintReadOnly, Category="Player Stats")
	float CurrentStamina = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Player Stats")
	float MaxXp = 100.0f;
	UPROPERTY(BlueprintReadOnly, Category="Player Stats")
	float CurrentXp = 0.0f;
	
	UPROPERTY(BlueprintReadOnly, Category="Player Stats")
	int32 CurrentLevel = 1;
};
