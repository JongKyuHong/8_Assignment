#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PerkDataAsset.generated.h"

UENUM(BlueprintType)
enum class EPerkRarity : uint8
{
	Silver, // 일반
	Gold,  // 희귀
	Prism  // 전설 느낌?, 보라~프리즘색
};

// 하나의 Perk이 가져야 하는 정보
USTRUCT(BlueprintType)
struct FPerkInfo 
{
	GENERATED_BODY()

	// Perk이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText PerkNames; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Icon; // 증강 이미지

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FLinearColor FrameColor; // 프레임색

	// 이 Perk과 연동된 Gameplay Effect클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UGameplayEffect> PerkEffectClass;
};

// 무작위로 3개의 특성을 가져옴 / 화면에 전달되는 단위
USTRUCT(BlueprintType)
struct FPerkRollResult
{
	GENERATED_BODY()

	// 선택된 3개의 퍽들
	UPROPERTY(BlueprintReadWrite)
	TArray<FPerkInfo> SelectedPerks;
};

UCLASS()
class ASSIGNMENT8_API UPerkDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	// 모든 Perk들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Perks")
	TArray<FPerkInfo> AllPerks;
};
