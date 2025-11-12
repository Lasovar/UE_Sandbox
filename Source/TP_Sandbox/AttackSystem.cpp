// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackSystem.h"

#include "TP_SandboxCharacter.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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
	Character = Cast<ATP_SandboxCharacter>(GetOuter());
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

void UAttackSystem::SwordAttack()
{
	if (!Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackSystem: No Owner found"));
		return;
	}

	if (IsAttacking)
	{
		AttackQueued = true;
		return;
	}

	CanAttack = false;
	IsAttacking = true;

	if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
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

void UAttackSystem::ApplyDamage(const FHitResult& Hit)
{
	if (!bCanApplyDamage)
		return;

	if (!Hit.GetActor()->ActorHasTag("Damageable"))
		return;

	Hit.GetActor()->TakeDamage(25.0f, FDamageEvent(), Character->GetController(), Character);
	bCanApplyDamage = false;

	GetWorld()->GetTimerManager().SetTimer(
		DamageRestTimerHandle,
		[this]
		{
			bCanApplyDamage = true;
		},
		0.2f,
		false
	);

	// Effects
	UGameplayStatics::PlayWorldCameraShake(GetOwner(), CameraShake, Hit.Location, 0.0f, 2000.0f, 1.0f);
	FRotator rotation = UKismetMathLibrary::MakeRotFromXY(Hit.Normal, Hit.Normal);
	UGameplayStatics::SpawnEmitterAtLocation(
		GetOwner(),
		BloodSplatter,
		Hit.Location,
		rotation,
		FVector(.4f),
		true,
		EPSCPoolMethod::AutoRelease
	);
}

void UAttackSystem::StartMeleeTrace()
{
	if (!bCanApplyDamage)
	{
		return;
	}

	FHitResult Hit;
	FVector attackPoint = Character->GetMeleeAttackPointLocation();
	if (UKismetSystemLibrary::SphereTraceSingle(
		GetOwner(),
		attackPoint,
		attackPoint,
		35.0f,
		TraceTypeQuery1,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		Hit,
		true
	))
	{
		ApplyDamage(Hit);
	}
}

void UAttackSystem::StartSwordTrace()
{
	if (!bCanApplyDamage)
	{
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(
		TraceTimerHandle,
		this,
		&UAttackSystem::SwordTracingLoop,
		0.1f,
		true
	);
}

void UAttackSystem::StopTrace()
{
	GetWorld()->GetTimerManager().ClearTimer(TraceTimerHandle);
}


void UAttackSystem::SwordTracingLoop()
{
	FHitResult Hit;
	if (UKismetSystemLibrary::SphereTraceSingle(
		GetOwner(),
		Character->GetSwordTopPointLocation(),
		Character->GetSwordBottomPointLocation(),
		12.0f,
		TraceTypeQuery1,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		Hit,
		true
	))
	{
		ApplyDamage(Hit);
	}
}
