// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackSystem.generated.h"


class ATP_SandboxCharacter;
struct FInputActionValue;
class UInputAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TP_SANDBOX_API UAttackSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttackSystem();

	void SwordAttack();
	void AllowNextAttack();

	void CancelAttack();

	void StartMeleeTrace();
	
	void StartSwordTrace();
	void StopTrace();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SwordTracingLoop();
	void ApplyDamage(const FHitResult& Hit);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	UPROPERTY(EditAnywhere, Category="Attack System")
	TArray<UAnimMontage*> AttackAnims;

	UPROPERTY(EditAnywhere, Category = "Attack System", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> CameraShake;
	
	UPROPERTY(EditAnywhere, Category = "Attack System", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BloodSplatter;
	
	TObjectPtr<ATP_SandboxCharacter> Character;
	
protected:
	bool IsAttacking;
	bool CanAttack = true;
	bool AttackQueued;
	uint32 CurrentAttackIndex = 0;
	FTimerHandle TraceTimerHandle;
	FTimerHandle DamageRestTimerHandle;
	bool bCanApplyDamage = true;
		
};
