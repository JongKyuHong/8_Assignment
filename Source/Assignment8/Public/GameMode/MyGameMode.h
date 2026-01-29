#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"

UENUM(BlueprintType)
enum class EWaveState : uint8
{
	Waiting,
	Selection,
	Active,
	Clear
};

UCLASS()
class ASSIGNMENT8_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AMyGameMode();

	UPROPERTY(VisibleAnywhere, Category="Wave")
	EWaveState CurrentState;


	void SetWaveState(EWaveState NewState);
};
