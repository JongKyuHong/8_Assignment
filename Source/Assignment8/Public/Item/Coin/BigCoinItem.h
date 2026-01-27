#pragma once

#include "CoreMinimal.h"
#include "Item/Coin/CoinItem.h"
#include "BigCoinItem.generated.h"


UCLASS()
class ASSIGNMENT8_API ABigCoinItem : public ACoinItem
{
	GENERATED_BODY()
public:
	ABigCoinItem();
	virtual void ActivateItem(AActor* Activator) override;
};
