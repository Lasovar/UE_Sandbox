// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_SandboxCharacter.h"

#include "BPI_Assassination.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MotionWarpingComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ATP_SandboxCharacter::ATP_SandboxCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	OnVaultMontageEnded.BindUObject(this, &ATP_SandboxCharacter::VaultMontageEnded);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ATP_SandboxCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATP_SandboxCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this,
		                                   &ATP_SandboxCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATP_SandboxCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}

void ATP_SandboxCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ATP_SandboxCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ATP_SandboxCharacter::VaultMotionWarp()
{
	float meshWorldZ = GetMesh()->GetComponentLocation().Z;
	if (CanWarp && meshWorldZ - 50 <= VaultLandPosition.Z && meshWorldZ + 50 >= VaultLandPosition.Z)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		SetActorEnableCollision(false);

		FMotionWarpingTarget motionStartTarget;
		motionStartTarget.Name = "VaultStart";
		motionStartTarget.Location = VaultStartPosition;
		motionStartTarget.Rotation = GetActorRotation();

		FMotionWarpingTarget motionMidTarget;
		motionMidTarget.Name = "VaultMid";
		motionMidTarget.Location = VaultMiddlePosition;
		motionMidTarget.Rotation = GetActorRotation();

		FMotionWarpingTarget motionEndTarget;
		motionEndTarget.Name = "VaultEnd";
		motionEndTarget.Location = VaultLandPosition;
		motionEndTarget.Rotation = GetActorRotation();

		auto warpingComponent = GetComponentByClass<UMotionWarpingComponent>();
		warpingComponent->AddOrUpdateWarpTarget(motionStartTarget);
		warpingComponent->AddOrUpdateWarpTarget(motionMidTarget);
		warpingComponent->AddOrUpdateWarpTarget(motionEndTarget);

		// play the vault montage
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			const float MontageLength = AnimInstance->Montage_Play(VaultMontage,
			                                                       1.0f,
			                                                       EMontagePlayReturnType::MontageLength,
			                                                       0.0f,
			                                                       true
			);

			// has the montage played successfully?
			if (MontageLength > 0.0f)
			{
				AnimInstance->Montage_SetEndDelegate(OnVaultMontageEnded, VaultMontage);
			}
		}
	}
}

void ATP_SandboxCharacter::VaultMontageEnded(UAnimMontage* vaultMontage, bool interrupted)
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	SetActorEnableCollision(true);
	CanWarp = false;
	VaultLandPosition = FVector(0.0, 0.0, -20000);
}

void ATP_SandboxCharacter::DoVault()
{
	const FName TraceTag("MyTraceTag");

	GetWorld()->DebugDrawTraceTag = TraceTag;

	FCollisionQueryParams CollisionParams;
	CollisionParams.TraceTag = TraceTag;

	FHitResult horiHit;
	FCollisionShape heightSphere = FCollisionShape::MakeSphere(5);

	for (int i = 0; i < 3; i++)
	{
		FVector start = GetActorLocation() + FVector(0, 0, i * 30);
		FVector end = start + GetActorForwardVector() * 180.0f;
		if (GetWorld()->SweepSingleByChannel(horiHit, start, end, FQuat::Identity, ECC_Visibility, heightSphere,
		                                     CollisionParams))
		{
			break;
		}
	}

	if (!horiHit.IsValidBlockingHit())
		return;

	FCollisionShape distanceSphere = FCollisionShape::MakeSphere(10);
	FHitResult vertHit;

	for (int i = 0; i < 6; i++)
	{
		FVector start = horiHit.Location + FVector(0, 0, 100) + GetActorForwardVector() * i * 50.0f;
		FVector end = start - FVector(0, 0, 100);

		if (GetWorld()->SweepSingleByChannel(vertHit, start, end, FQuat::Identity, ECC_Visibility, distanceSphere,
		                                     CollisionParams))
		{
			if (i == 0)
			{
				VaultStartPosition = vertHit.Location;
				DrawDebugSphere(GetWorld(), VaultStartPosition, 10, 12, FColor::Red, true, 0.5f);
			}

			VaultMiddlePosition = vertHit.Location;
			CanWarp = true;
			DrawDebugSphere(GetWorld(), VaultMiddlePosition, 10, 12, FColor::Red, true, 0.5f);
		}
		else
		{
			FHitResult vaultEnd;
			FVector vaultTraceStart = start + GetActorForwardVector() * 80.0f;
			FVector vaultTraceEnd = vaultTraceStart + FVector(0, 0, -1000);
			if (GetWorld()->LineTraceSingleByChannel(vaultEnd, vaultTraceStart, vaultTraceEnd, ECC_Visibility,
			                                         CollisionParams))
			{
				VaultLandPosition = vaultEnd.Location;
				break;
			}
		}
	}

	VaultMotionWarp();
}

void ATP_SandboxCharacter::DoAssassinate()
{
	TArray<AActor*> overlaps;
	GetOverlappingActors(overlaps);

	for (const auto& Actor : overlaps)
	{
		if (Actor && Actor->Implements<UBPI_Assassination>())
		{
			if (IBPI_Assassination* Assassination = Cast<IBPI_Assassination>(Actor))
			{
				auto [location, rotator] = Assassination->StealthBackAssassin();

				if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
				{
					AnimInstance->Montage_Play(AssassinationMontage);

					FMotionWarpingTarget motionAssassinateTarget;
					motionAssassinateTarget.Name = "AssassinationWarp";
					motionAssassinateTarget.Location = location;
					motionAssassinateTarget.Rotation = rotator;

					auto warpingComponent = GetComponentByClass<UMotionWarpingComponent>();
					warpingComponent->AddOrUpdateWarpTarget(motionAssassinateTarget);

					break;
				}
			}

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Couldn't Cast interface"));
			}
		}
	}
}

void ATP_SandboxCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ATP_SandboxCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ATP_SandboxCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();

	this->UnCrouch();
}

void ATP_SandboxCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}
