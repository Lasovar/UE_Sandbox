// Fill out your copyright notice in the Description page of Project Settings.


#include "Notfiy_SwordTraceLoop.h"

#include "AttackSystem.h"

void UNotfiy_SwordTraceLoop::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                         float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (auto Owner = MeshComp->GetOwner())
	{
		if (auto AttackSystem = Owner->FindComponentByClass<UAttackSystem>())
		{
			AttackSystem->StartSwordTrace();
		}
	}
}

void UNotfiy_SwordTraceLoop::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	if (auto Owner = MeshComp->GetOwner())
	{
		if (auto AttackSystem = Owner->FindComponentByClass<UAttackSystem>())
		{
			AttackSystem->StopTrace();
		}
	}
}
