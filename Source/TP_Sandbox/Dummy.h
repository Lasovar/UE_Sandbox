// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BPI_Assassination.h"
#include "GameFramework/Character.h"
#include "Dummy.generated.h"

class UHealthComponent;
class USphereComponent;
class UWidgetComponent;

UCLASS()
class TP_SANDBOX_API ADummy : public ACharacter, public IBPI_Assassination
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADummy();

	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnDeath();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual UAnimMontage* GetAssassinatedMontage_Implementation() const override;

public:
	virtual TTuple<FVector, FRotator> Assassinate();
	
protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
private:
	UPROPERTY(Category=Stats, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UHealthComponent> Health;
	
	UPROPERTY(Category=Assassination, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> AssassinRef;

	UPROPERTY(Category=Assassination, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> AssassinationWidget;
	
	UPROPERTY(Category=Assassination, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> AssassinationRadius;
	
	UPROPERTY(Category=UI, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> UIWidget;

protected:
	UPROPERTY(EditAnywhere, Category="Assassination")
	UAnimMontage* AssassinatedMontage;
	
	UPROPERTY(EditAnywhere, Category="Assassination")
	USoundBase* DeathSound;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<UAnimMontage*> HitReacts;

private:
	FTimerHandle RagdollTimerHandle;
};
