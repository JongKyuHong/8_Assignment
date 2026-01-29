#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PerkDataAsset.generated.h"


UENUM(BlueprintType)
enum class EPerkRarity : uint8
{
	Silver,
	Gold,
	Prism
};

USTRUCT(BlueprintType)
struct FPerkRarityValues
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Silver = 1.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Gold = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Prism = 1.4f;
};

USTRUCT(BlueprintType)
struct FPerkInfo 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText PerkNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPerkRarityValues Multipliers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UGameplayEffect> PerkEffectClass;
};

USTRUCT(BlueprintType)
struct FPerkRollResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<FPerkInfo> SelectedPerks;

	UPROPERTY(BlueprintReadOnly)
	EPerkRarity RolledRarity;
};

UCLASS()
class ASSIGNMENT8_API UPerkDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Perks")
	TArray<FPerkInfo> AllPerks;

	UPROPERTY(EditAnywhere, Category="Chances")
	float SilverChance = 40.f;

	UPROPERTY(EditAnywhere, Category = "Chances")
	float GoldChance = 35.f;

	UPROPERTY(EditAnywhere, Category = "Chances")
	float PrismChance = 25.f;
};
