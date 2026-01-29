#include "DataAsset/PerkManagerComponent.h"

UPerkManagerComponent::UPerkManagerComponent()
{

}


FPerkRollResult UPerkManagerComponent::RollPerkOptions(UPerkDataAsset* DataAsset)
{
	FPerkRollResult Results;
	if (!DataAsset) return Results;

	Results.RolledRarity = GetRandomRarity();

	TArray<FPerkInfo> Pool = DataAsset->AllPerks;

	while (Results.SelectedPerks.Num() < 3 && Pool.Num() > 0)
	{
		int32 index = FMath::RandRange(0, Pool.Num() - 1);

		Results.SelectedPerks.Add(Pool[index]);
		Pool.RemoveAt(index);
	}

	return Results;
}

EPerkRarity UPerkManagerComponent::GetRandomRarity()
{
	int32 Roll = FMath::RandRange(1, 100);
	if (Roll <= 40) return EPerkRarity::Silver;
	if (Roll <= 75) return EPerkRarity::Gold;
	return EPerkRarity::Prism;
}
