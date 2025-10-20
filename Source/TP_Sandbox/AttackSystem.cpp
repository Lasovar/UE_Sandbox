// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSystem.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UAttackSystem::UAttackSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttackSystem::BeginPlay()
{
	Super::BeginPlay();
	
	
	// ...
	Mesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
}

void UAttackSystem::SwordTracingLoop()
{
	FHitResult Hit;
	UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		FVector(0, 0, 0),
		FVector(0, 0, 0),
		50.0f,
		TraceTypeQuery1,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		Hit,
		true
	);
}

void UAttackSystem::SwordAttack()
{
	if (!Mesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackSystem: No Mesh found on Owner"));
		return;
	}

	if (IsAttacking)
	{
		AttackQueued = true;
		return;
	}

	CanAttack = false;
	IsAttacking = true;

	if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
	{
		if (AttackAnims.IsValidIndex(CurrentAttackIndex))
		{
			UAnimMontage* AttackMontage = AttackAnims[CurrentAttackIndex];
			AnimInstance->Montage_Play(AttackMontage);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AttackSystem: Invalid Attack Animation Index"));
			IsAttacking = false;
		}
	}
}

void UAttackSystem::AllowNextAttack()
{
	CanAttack = true;
	IsAttacking = false;
	CurrentAttackIndex = (CurrentAttackIndex + 1) % AttackAnims.Num();
}

void UAttackSystem::CancelAttack()
{
	IsAttacking = false;
	CurrentAttackIndex = 0;
	AttackQueued = false;
}

void UAttackSystem::StartSwordTrace()
{
	GetWorld()->GetTimerManager().SetTimer(
		SwordTraceTimerHandle,
		this,
		&UAttackSystem::SwordTracingLoop,
		0.1f,
		true
	);
}


// Called every frame
void UAttackSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (CanAttack && AttackQueued && !IsAttacking)
	{
		SwordAttack();
		AttackQueued = false;
	}
}
