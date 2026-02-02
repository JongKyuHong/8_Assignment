#pragma once

#include "CoreMinimal.h"
#include "Item/BaseItem.h"
#include "MineItem.generated.h"

class UGameplayEffect;

UCLASS()
class ASSIGNMENT8_API AMineItem : public ABaseItem
{
	GENERATED_BODY()
public:
    AMineItem();


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
    USphereComponent* ExplosionCollision;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
    UParticleSystem* ExplosionParticle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
    USoundBase* ExplosionSound;

    bool bHasExploded;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
    float ExplosionDelay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
    float ExplosionRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
    int ExplosionDamage;

    FTimerHandle ExplosionTimerHandle;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mine|GAS")
    TSubclassOf<UGameplayEffect> ExplosionDamageGE;

    virtual void ActivateItem(AActor* Activator) override;

    void Explode();
};
