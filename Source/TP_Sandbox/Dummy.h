// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BPI_Assassination.h"
#include "GameFramework/Character.h"
#include "Dummy.generated.h"

class USphereComponent;
class UWidgetComponent;

UCLASS()
class TP_SANDBOX_API ADummy : public ACharacter, public IBPI_Assassination
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADummy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
	UPROPERTY(Category=Assassination, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> AssassinRef;

	UPROPERTY(Category=Assassination, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> AssassinationWidget;
	
	UPROPERTY(Category=Assassination, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> AssassinationRadius;

protected:
	UPROPERTY(EditAnywhere, Category="Assassination")
	UAnimMontage* AssassinatedMontage;
	
	UPROPERTY(EditAnywhere, Category="Assassination")
	USoundBase* DeathSound;

private:
	FTimerHandle RagdollTimerHandle;
};
