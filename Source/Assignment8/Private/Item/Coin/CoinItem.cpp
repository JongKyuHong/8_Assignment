#include "Item/Coin/CoinItem.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/MyAttributeSet.h"
#include "GameState/MyGameState.h"

ACoinItem::ACoinItem()
{
    PointValue = 0;
	ExpValue = 0;
    ItemType = "DefaultCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

    // 동전과 오버랩된 Activator에게 ASC가 있는지 
    UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Activator);
    

    if (ASC && ExpEffectClass)
    {
        // 경험치율 가져오기, 매크로덕에 Get으로 AttributeSet에서 가져올 수 있음
        float GainRate = ASC->GetNumericAttribute(UMyAttributeSet::GetExpGainRateAttribute());

        // 경험치율 적용
        float FinalExp = ExpValue * GainRate;

        // 에디터에서 설정한 ExpEffectClass를 바탕으로 실제 적용할 데이터 Spec만들기
        FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(ExpEffectClass, 1.0f, ASC->MakeEffectContext());
        if (SpecHandle.IsValid())
        {
            // C++에서 계산한 경험치를 GE에 넣음
            SpecHandle.Data.Get()->SetSetByCallerMagnitude(
                FGameplayTag::RequestGameplayTag(FName("Data.Exp")),
                FinalExp
            );
            // 캐릭터에도 적용시켜달라고 ASC에게 명령
            ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), ASC);
        }
    }
    else if (!ExpEffectClass)
    {
        UE_LOG(LogTemp, Error, TEXT("ExpEffectClass 없음"));
    }

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (UWorld* World = GetWorld())
		{
			if (AMyGameState* GameState = World->GetGameState<AMyGameState>())
			{
				GameState->AddScore(PointValue);
				GameState->OnCoinCollected();
			}
		}
		DestroyItem();
	}
}