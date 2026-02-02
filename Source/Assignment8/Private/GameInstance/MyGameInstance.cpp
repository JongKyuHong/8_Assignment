#include "GameInstance/MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
    ResetAllStats();
}

void UMyGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated: %d"), TotalScore);
}

void UMyGameInstance::ResetAllStats()
{
    TotalScore = 0;
    CurrentLevelIndex = 0;
    Health = 100.0f;
    MaxHealth = 100.0f;
    MoveSpeed = 600.0f;
    JumpZVelocity = 300.0f;
    CharacterScale = 1.0f;
    ExpGainRate = 1.0f;
    Level = 1.0f;
    Exp = 0.0f;
    MaxExp = 100.0f;
}