#pragma once

#include "CoreMinimal.h"
#include "Item/Coin/CoinItem.h"
#include "SmallCoinItem.generated.h"


UCLASS()
class ASSIGNMENT8_API ASmallCoinItem : public ACoinItem
{
	GENERATED_BODY()
public:
	ASmallCoinItem();

	virtual void ActivateItem(AActor* Activator) override;
};
