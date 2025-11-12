// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthUpdated, float, currentHealth, float, maxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TP_SANDBOX_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthUpdated OnHealthUpdated;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDeath OnDeath;
	
	UFUNCTION(BlueprintCallable, Category="Health")
	virtual bool DecreaseHealth(float Amount);
	UFUNCTION(BlueprintCallable, Category="Health")
	virtual void IncreaseHealth(float Amount);
	UFUNCTION(BlueprintCallable, Category="Health")
	virtual void IncreaseMaxHealth(float Amount);
	
	UFUNCTION(BlueprintCallable, Category="Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	
	UFUNCTION(BlueprintCallable, Category="Health")
	FORCEINLINE bool IsDead() const { return CurrentHealth <= 0; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Health")
	float MaxHealth = 100.0f;
	
	float CurrentHealth = 0.0f;
};
