#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WaveDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FWaveItemConfig
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, Category="Wave")
	int32 WaveIndex;

	UPROPERTY(EditAnywhere, Category="Wave")
	TArray<TSubclassOf<AActor>> SpawnableItems;
};

UCLASS()
class ASSIGNMENT8_API UWaveDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category="Wave")
	TArray<FWaveItemConfig> WaveConfigs;
};
