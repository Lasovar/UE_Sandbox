// Fill out your copyright notice in the Description page of Project Settings.


#include "Dummy.h"

#include "HealthComponent.h"
#include "TP_SandboxCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADummy::ADummy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = CreateDefaultSubobject<UHealthComponent>("Health Component");
	Health->OnDeath.AddDynamic(this, &ADummy::OnDeath);
	
	AssassinRef = CreateOptionalDefaultSubobject<USkeletalMeshComponent>("AssassinRef");
	AssassinRef->SetupAttachment(GetCapsuleComponent());

	AssassinationWidget = CreateOptionalDefaultSubobject<UWidgetComponent>("AssassinationWidget");
	AssassinationWidget->SetupAttachment(GetCapsuleComponent());

	AssassinationRadius = CreateDefaultSubobject<USphereComponent>("AssassinationRadius");
	if (AssassinationRadius)
	{
		AssassinationRadius->SetupAttachment(GetCapsuleComponent());
		AssassinationRadius->SetGenerateOverlapEvents(true);

		AssassinationRadius->OnComponentBeginOverlap.AddDynamic(this, &ADummy::OnOverlapBegin);
		AssassinationRadius->OnComponentEndOverlap.AddDynamic(this, &ADummy::OnOverlapEnd);
	}

	UIWidget = CreateDefaultSubobject<UWidgetComponent>("World UI");
	UIWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADummy::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &ADummy::OnTakeDamage);
	
	if (AssassinationWidget)
	{
		AssassinationWidget->SetVisibility(false);
	}
}

void ADummy::OnDeath()
{
	GetMesh()->SetSimulatePhysics(true);
	UIWidget->SetVisibility(false);
}

// Called every frame
void ADummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const APlayerController* PC = UGameplayStatics::GetPlayerController(GetController(), 0);
	AssassinationWidget->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(
		AssassinationWidget->GetComponentLocation(),
		PC->PlayerCameraManager->GetCameraLocation()
	));
	

	UIWidget->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(
		UIWidget->GetComponentLocation(),
		PC->PlayerCameraManager->GetCameraLocation()	
	));
}

void ADummy::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                          AController* InstigatedBy, AActor* DamageCauser)
{
	Health->DecreaseHealth(Damage);
	int hitIndex = UKismetMathLibrary::RandomInteger(HitReacts.Num());
	GetMesh()->GetAnimInstance()->Montage_Play(HitReacts[hitIndex]);

	LaunchCharacter(DamageCauser->GetActorForwardVector() * 700.0f, false, false);
}

// Called to bind functionality to input
void ADummy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UAnimMontage* ADummy::GetAssassinatedMontage_Implementation() const
{
	return AssassinatedMontage;
}

TTuple<FVector, FRotator> ADummy::Assassinate()
{
	const auto ReturnTransform = TTuple<FVector, FRotator>(AssassinRef->GetComponentLocation(), GetActorRotation());

	auto MeshComponent = GetMesh();
	if (!MeshComponent)
		return ReturnTransform;

	// Disable Assassination Widget
	AssassinationWidget->SetVisibility(false);

	// Assassination Animation
	if (UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance())
	{
		if (UAnimMontage* Montage = AssassinatedMontage)
		{
			AnimInstance->Montage_Play(Montage);
		}
	}

	// Ragdoll
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[this]
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
		},
		1.6f,
		false
	);

	GetWorld()->GetTimerManager().SetTimer(
		RagdollTimerHandle,
		[MeshComponent]
		{
			MeshComponent->SetSimulatePhysics(true);
		},
		2.0f,
		false
	);

	return ReturnTransform;
}

void ADummy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                            int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ATP_SandboxCharacter>(OtherActor))
	{
		AssassinationWidget->SetVisibility(true);
	}
}

void ADummy::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                          int32 OtherBodyIndex)
{
	if (Cast<ATP_SandboxCharacter>(OtherActor))
	{
		AssassinationWidget->SetVisibility(false);
	}
}
