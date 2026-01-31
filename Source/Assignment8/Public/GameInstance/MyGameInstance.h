#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

UCLASS()
class ASSIGNMENT8_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMyGameInstance();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	float MoveSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	float JumpZVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	float CharacterScale;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	float ExpGainRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	float Level;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	float Exp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	float MaxExp;

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);

	void ResetAllStats();
};
