// Fill out your copyright notice in the Description page of Project Settings.


#include "Dummy.h"

#include "TP_SandboxCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADummy::ADummy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
}

// Called when the game starts or when spawned
void ADummy::BeginPlay()
{
	Super::BeginPlay();

	if (AssassinationWidget)
	{
		AssassinationWidget->SetVisibility(false);
	}
}

// Called every frame
void ADummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AssassinationWidget->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(
		AssassinationWidget->GetComponentLocation(),
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation()
	));
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
