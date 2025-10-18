// Fill out your copyright notice in the Description page of Project Settings.

#include "BPI_Assassination.h"

#include "Dummy.h"
#include "GameFramework/Character.h"

// Add default functionality here for any IBPI_Assassination functions that are not pure virtual.
TTuple<FVector, FRotator> IBPI_Assassination::StealthBackAssassin()
{
	ADummy* Actor = Cast<ADummy>(_getUObject());
	if (!Actor)
	{
		return TTuple<FVector, FRotator>();
	}

	return Actor->Assassinate();
}
