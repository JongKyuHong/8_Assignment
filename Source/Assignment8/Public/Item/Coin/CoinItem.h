#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h"
#include "CoinItem.generated.h"

UCLASS()
class ASSIGNMENT8_API ACoinItem : public ABaseItem
{
	GENERATED_BODY()
public:
	ACoinItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	int32 PointValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ExpValue;

	UPROPERTY(EditAnywhere, Category = "GAS")
	TSubclassOf<class UGameplayEffect> ExpEffectClass;

	virtual void ActivateItem(AActor* Activator) override;
};
