#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h"
#include "PoisionPotion.generated.h"

UCLASS()
class ASSIGNMENT8_API APoisionPotion : public ABaseItem
{
	GENERATED_BODY()
protected:
	virtual void ActivateItem(AActor* Activator) override;
};
