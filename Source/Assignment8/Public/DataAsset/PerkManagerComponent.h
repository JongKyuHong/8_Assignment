#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PerkDataAsset.h"
#include "PerkManagerComponent.generated.h"

struct FPerkInfo;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASSIGNMENT8_API UPerkManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPerkManagerComponent();


	FPerkRollResult RollPerkOptions(UPerkDataAsset* DataAsset);
	EPerkRarity GetRandomRarity();
};
