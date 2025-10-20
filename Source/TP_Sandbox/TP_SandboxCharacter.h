// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Utility/Timer.h"
#include "TP_SandboxCharacter.generated.h"

class UAttackSystem;
class UPlayerStats;
class UWidgetComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class ATP_SandboxCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Stats", meta = (AllowPrivateAccess = "true"))
	UPlayerStats* Stats;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack System", meta = (AllowPrivateAccess = "true"))
	UAttackSystem* AttackSystem;

	bool CanWarp;
	FVector VaultStartPosition = FVector(0, 0, 0);
	FVector VaultMiddlePosition = FVector(0, 0, 0);
	FVector VaultLandPosition = FVector(0, 0, 0);
	
protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* JumpAction;
	
	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* SprintAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* MouseLookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* AttackAction;

public:

	/** Constructor */
	ATP_SandboxCharacter();	

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

protected:
	void StartSprint(const FInputActionValue& Value);
	void StopSprint(const FInputActionValue& Value);

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	void Attack(const FInputActionValue& Value);
	
	void VaultMotionWarp();

	void VaultMontageEnded(UAnimMontage* vaultMontage, bool interrupted);

	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnDeath();
	
	UFUNCTION()
	void OnStaminaUpdated(float currentStamina, float previousStamina, float maxStamina);
public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoVault();
	
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoAssassinate();

	UPROPERTY(EditAnywhere, Category="Character Movement: Walking")
	float SprintingSpeed = 750.0f;
	
	UPROPERTY(EditAnywhere, Category="HUD")
	TObjectPtr<UUserWidget> WidgetHUD;
	
	UPROPERTY(EditAnywhere, Category="Vault")
	UAnimMontage* VaultMontage;
	
	UPROPERTY(EditAnywhere, Category="Assassination")
	UAnimMontage* AssassinationMontage;
	
	UPROPERTY(EditAnywhere, Category="Character Movement: Walking")
	float StaminaDepletingSpeed = 2.0f;
	UPROPERTY(EditAnywhere, Category="Character Movement: Walking")
	float StaminaRegenDelay = 1.0f;
	UPROPERTY(EditAnywhere, Category="Character Movement: Walking")
	float StaminaRegenRate = 3.0f;
protected:
	FOnMontageEnded OnVaultMontageEnded;
	bool IsSprinting = false;
	FTimer StaminaRegenTimer;
	
public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

