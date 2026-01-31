#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PerkDataAsset.h"
#include "PerkManagerComponent.generated.h"

struct FPerkInfo;

// meta=BlueprintSpawnableComponent 블루프린트에서 사용할 수 있게
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASSIGNMENT8_API UPerkManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPerkManagerComponent();

	// 3개의 퍽을 랜덤으로 정하는 함수
	FPerkRollResult RollPerkOptions(UPerkDataAsset* DataAsset);
};
