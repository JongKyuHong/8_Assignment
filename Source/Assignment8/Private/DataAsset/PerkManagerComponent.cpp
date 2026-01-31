#include "DataAsset/PerkManagerComponent.h"

UPerkManagerComponent::UPerkManagerComponent()
{

}

FPerkRollResult UPerkManagerComponent::RollPerkOptions(UPerkDataAsset* DataAsset)
{
	FPerkRollResult Results;
	if (!DataAsset) return Results;

	TArray<FPerkInfo> Pool = DataAsset->AllPerks;

	while (Results.SelectedPerks.Num() < 3 && Pool.Num() > 0)
	{
		int32 index = FMath::RandRange(0, Pool.Num() - 1);

		Results.SelectedPerks.Add(Pool[index]);
		Pool.RemoveAt(index);
	}

	return Results;
}