#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h"
#include "SlowPotion.generated.h"

class UGameplayEffect;

UCLASS()
class ASSIGNMENT8_API ASlowPotion : public ABaseItem
{
	GENERATED_BODY()

protected:
	virtual void ActivateItem(AActor* Activator) override;
};
