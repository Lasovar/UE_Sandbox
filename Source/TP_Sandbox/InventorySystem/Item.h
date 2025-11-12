#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"  
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None,
	Melee,
	Shield,
	Ranged,
	Armor
};

USTRUCT(BlueprintType)
struct FItem : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties")
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties")
	float Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties")
	int32 RequiredLevel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties")
	TObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties")
	TObjectPtr<UStaticMesh> Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties")
	EItemType Type;
};
