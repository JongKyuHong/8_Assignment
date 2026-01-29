#include "GameState/MyGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Spawn/SpawnVolume.h"
#include "Item/Coin/CoinItem.h"
#include "Controller/MyPlayerController.h"
#include "Character/MyCharacter.h"
#include "GameInstance/MyGameInstance.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"

AMyGameState::AMyGameState()
{
    Score = 0;
    SpawnedCoinCount = 0;
    CollectedCoinCount = 0;
    LevelDuration = 30.f;
    CurrentLevelIndex = 0;
    MaxLevels = 3;
}

void AMyGameState::BeginPlay()
{
    Super::BeginPlay();
    StartLevel();
    UpdateHUD();

    GetWorldTimerManager().SetTimer(
        HUDUpdateTimerHandle,
        this,
        &AMyGameState::UpdateHUD,
        0.1f,
        true
    );
}

int32 AMyGameState::GetScore() const
{
    return Score;
}

void AMyGameState::AddScore(int32 Amount)
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
        if (MyGameInstance)
        {
            MyGameInstance->AddToScore(Amount);
        }
    }
}


void AMyGameState::StartLevel()
{
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
        {
            MyPlayerController->ShowGameHUD();
        }
    }


    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
        if (MyGameInstance)
        {
            CurrentLevelIndex = MyGameInstance->CurrentLevelIndex;
        }
    }

    SpawnedCoinCount = 0;
    CollectedCoinCount = 0;

    TArray<AActor*> FoundVolumes;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

    const int32 ItemToSpawn = 40;

    for (int32 i = 0; i < ItemToSpawn; i++) 
    {
        if (FoundVolumes.Num() > 0) 
        {
            ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
            if (SpawnVolume)
            {
                AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
                UE_LOG(LogTemp, Warning, TEXT("Spawned Class: %s"), *SpawnedActor->GetClass()->GetName());
                if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
                {
                    SpawnedCoinCount++;
                }
            }
        }
    }

    GetWorldTimerManager().SetTimer(
        LevelTimerHandle,
        this,
        &AMyGameState::OnLevelTimeUp,
        LevelDuration,
        false
    );
}

void AMyGameState::OnLevelTimeUp()
{
    // 얻은 코인의 개수가 스폰된것과 같으면 종료
    EndLevel();
}

void AMyGameState::OnCoinCollected()
{
    CollectedCoinCount++;

    if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
    {
        EndLevel();
    }
}

void AMyGameState::EndLevel()
{
    GetWorldTimerManager().ClearTimer(LevelTimerHandle);

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
        if (MyGameInstance)
        {
            AddScore(Score);
            CurrentLevelIndex++;
            MyGameInstance->CurrentLevelIndex = CurrentLevelIndex;

            if (CurrentLevelIndex >= MaxLevels)
            {
                OnGameOver();
                return;
            }

            if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
            {
                UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
            } else
            {
                OnGameOver();
            }
        }
    }
}

void AMyGameState::OnGameOver()
{
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
        {
            MyPlayerController->SetPause(true);
            MyPlayerController->ShowMainMenu(true);
        }
    }
}

void AMyGameState::UpdateHUD()
{
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController);
        {
            if (UUserWidget* HUDWidget = MyPlayerController->GetHUDWidget())
            {
                if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
                {
                    float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
                    TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.f"), RemainingTime)));
                }

                if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
                {
                    if (UGameInstance* GameInstance = GetGameInstance())
                    {
                        UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
                        if (MyGameInstance)
                        {
                            ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), MyGameInstance->TotalScore)));
                        }
                    }
                }

                if (UProgressBar* HealthBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("HealthBar"))))
                {
                    if (APawn* PlayerPawn = MyPlayerController->GetPawn())
                    {
                        if (AMyCharacter* PlayerCharacter = Cast<AMyCharacter>(PlayerPawn))
                        {
                            float CurrentHP = PlayerCharacter->GetHealth();
                            float MaxHP = PlayerCharacter->MaxHealth;

                            float HealthRatio = MaxHP > 0 ? CurrentHP / MaxHP : 0.f;

                            HealthBar->SetPercent(HealthRatio);
                        }
                    }
                }

                if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
                {
                    LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
                }
            }
        }
    }
}
