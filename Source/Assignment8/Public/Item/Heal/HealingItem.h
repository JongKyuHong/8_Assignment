#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h"
#include "HealingItem.generated.h"

UCLASS()
class ASSIGNMENT8_API AHealingItem : public ABaseItem
{
	GENERATED_BODY()
public:
	AHealingItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float HealAmount;

	virtual void ActivateItem(AActor* Activator) override;
};
