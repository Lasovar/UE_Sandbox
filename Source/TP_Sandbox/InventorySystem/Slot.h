#pragma once

#include "CoreMinimal.h"
#include "Slot.generated.h"

USTRUCT(BlueprintType)
struct FSlot
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slots")
	FDataTableRowHandle Item;
};
