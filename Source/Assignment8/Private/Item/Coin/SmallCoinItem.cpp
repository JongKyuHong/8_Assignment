#include "Item/Coin/SmallCoinItem.h"

ASmallCoinItem::ASmallCoinItem()
{
    PointValue = 10;
    ExpValue = 10;
    ItemType = "SmallCoin";
}

void ASmallCoinItem::ActivateItem(AActor* Activator)
{
    Super::ActivateItem(Activator);
}
