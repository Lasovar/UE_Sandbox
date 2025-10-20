// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStats.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthUpdated, float, currentHealth, float, maxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStaminaUpdated, float, currentStamina, float, previousStamina, float, maxStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnXpUpdated, float, currentXp, float, maxXp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelUpdate, int32, currentLevel);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TP_SANDBOX_API UPlayerStats : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStats();

	float GetCurrentHealth() const { return CurrentHealth; }
	float GetCurrentStamina() const { return CurrentStamina; }
public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthUpdated OnHealthUpdated;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDeath OnDeath;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStaminaUpdated OnStaminaUpdated;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnLevelUpdate OnLevelUpdate;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnXpUpdated OnXpUpdated;

	UFUNCTION(BlueprintCallable, Category="Health")
	virtual bool DecreaseHealth(float Amount);
	UFUNCTION(BlueprintCallable, Category="Health")
	virtual void IncreaseHealth(float Amount);
	UFUNCTION(BlueprintCallable, Category="Health")
	virtual void IncreaseMaxHealth(float Amount);
	UFUNCTION(BlueprintCallable, Category="Health")
	virtual bool IsDead() const { return CurrentHealth <= 0; }

	UFUNCTION(BlueprintCallable, Category="Stamina")
	virtual bool DecreaseStamina(float Amount);
	UFUNCTION(BlueprintCallable, Category="Stamina")
	virtual void IncreaseStamina(float Amount);
	UFUNCTION(BlueprintCallable, Category="Stamina")
	virtual void IncreaseMaxStamina(float Amount);
	
	UFUNCTION(BlueprintCallable, Category="XP")
	virtual void IncreaseXp(float Amount);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Health")
	float MaxHealth = 100.0f;
	UPROPERTY(BlueprintReadOnly, Category="Health")
	float CurrentHealth = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Stamina")
	float MaxStamina = 100.0f;
	UPROPERTY(BlueprintReadOnly, Category="Stamina")
	float CurrentStamina = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="XP")
	float MaxXp = 100.0f;
	UPROPERTY(BlueprintReadOnly, Category="XP")
	float CurrentXp = 0.0f;
	
	UPROPERTY(BlueprintReadOnly, Category="XP")
	int32 CurrentLevel = 1;
};
