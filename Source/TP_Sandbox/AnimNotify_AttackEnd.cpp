// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AttackEnd.h"

#include "AttackSystem.h"

void UAnimNotify_AttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                   const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (auto Owner = MeshComp->GetOwner())
	{
		if (auto AttackSystem = Owner->FindComponentByClass<UAttackSystem>())
		{
			AttackSystem->CancelAttack();
		}
	}
}
