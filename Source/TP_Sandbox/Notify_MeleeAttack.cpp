// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify_MeleeAttack.h"

#include "AttackSystem.h"


void UNotify_MeleeAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                 const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	
	if (auto Owner = MeshComp->GetOwner())
	{
		if (auto AttackSystem = Owner->FindComponentByClass<UAttackSystem>())
		{
			AttackSystem->StartMeleeTrace();
		}
	}
}
