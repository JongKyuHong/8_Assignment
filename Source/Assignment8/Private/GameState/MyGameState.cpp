#include "GameState/MyGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Spawn/SpawnVolume.h"
#include "Item/Coin/CoinItem.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/MyAttributeSet.h"
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
    WaveDuration = 30.f;
    CurrentLevelIndex = 0;
    MaxLevels = 3;
    CurrentWaveIndex = 0;
    MaxWaves = 3;
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

        if (UGameInstance* GameInstance = GetGameInstance())
        {
            UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
            if (MyGameInstance)
            {
                CurrentLevelIndex = MyGameInstance->CurrentLevelIndex;
                if (CurrentLevelIndex == 0)
                {
                    MyGameInstance->ResetAllStats();
                }
                if (APawn* PlayerPawn = PlayerController->GetPawn())
                {
                    if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerPawn))
                    {
                        UMyAttributeSet* AS = const_cast<UMyAttributeSet*>(Cast<UMyAttributeSet>(ASC->GetAttributeSet(UMyAttributeSet::StaticClass())));

                        if (AS)
                        {
                            AS->SetHealth(MyGameInstance->Health);
                            AS->SetMaxHealth(MyGameInstance->MaxHealth);
                            AS->SetMoveSpeed(MyGameInstance->MoveSpeed);
                            AS->SetJumpZVelocity(MyGameInstance->JumpZVelocity);
                            AS->SetCharacterScale(MyGameInstance->CharacterScale);
                            AS->SetExpGainRate(MyGameInstance->ExpGainRate);
                            AS->SetLevel(MyGameInstance->Level);
                            AS->SetExp(MyGameInstance->Exp);
                            AS->SetMaxExp(MyGameInstance->MaxExp);
                        }
                    }
                }
            }
        }
    }

    StartWave();
}



void AMyGameState::OnLevelTimeUp()
{
    EndLevel();
}

void AMyGameState::OnCoinCollected()
{
    CollectedCoinCount++;

    //if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
    if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
    {
        EndWave();
        //EndLevel();
    }
}

void AMyGameState::StartWave()
{
    SpawnedCoinCount = 0;
    CollectedCoinCount = 0;

    TArray<AActor*> FoundVolumes;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

    const int32 ItemToSpawn = 10;

    for (int32 i = 0; i < ItemToSpawn; i++)
    {
        if (FoundVolumes.Num() > 0)
        {
            ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
            if (SpawnVolume)
            {
                AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
                if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
                {
                    SpawnedCoinCount++;
                }
            }
        }
    }

    GetWorldTimerManager().SetTimer(
        WaveTimerHandle,
        this,
        &AMyGameState::OnWaveTimeUp,
        WaveDuration,
        false
    );

    //ShowPerkMenu();
}

void AMyGameState::ShowPerkMenu() const
{
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
        {
            MyPlayerController->ShowPerkUI();
        }
    }
}

void AMyGameState::OnWaveTimeUp()
{
    EndWave();
}

void AMyGameState::EndWave()
{
    GetWorldTimerManager().ClearTimer(WaveTimerHandle);
    // 3웨이브를 다 겪었다면 다음 레벨로
    CurrentWaveIndex++;
    if (CurrentWaveIndex >= MaxWaves) 
    {
        EndLevel();
    }
    else 
    {
        StartWave();
    }
}

void AMyGameState::EndLevel()
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
        if (MyGameInstance)
        {
            AddScore(Score);
            CurrentLevelIndex++;
            MyGameInstance->CurrentLevelIndex = CurrentLevelIndex;

            if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
            {
                if (APawn* PlayerPawn = PlayerController->GetPawn()) 
                {
                    if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(PlayerPawn))
                    {
                        const UMyAttributeSet* AttributeSet = Cast<UMyAttributeSet>(ASC->GetAttributeSet(UMyAttributeSet::StaticClass()));
                        if (AttributeSet)
                        {
                            MyGameInstance->Health = AttributeSet->GetHealth();
                            MyGameInstance->MaxHealth = AttributeSet->GetMaxHealth();
                            MyGameInstance->MoveSpeed = AttributeSet->GetMoveSpeed();
                            MyGameInstance->JumpZVelocity = AttributeSet->GetJumpZVelocity();
                            MyGameInstance->CharacterScale = AttributeSet->GetCharacterScale();
                            MyGameInstance->ExpGainRate = AttributeSet->GetExpGainRate();
                            MyGameInstance->Level = AttributeSet->GetLevel();
                            MyGameInstance->Exp = AttributeSet->GetExp();
                            MyGameInstance->MaxExp = AttributeSet->GetMaxExp();
                        }
                    }
                }
            }
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
                    float RemainingTime = GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle);
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
                    LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d - %d"), CurrentLevelIndex + 1, CurrentWaveIndex+1)));
                }

                // 캐릭터 레벨이랑 체력은 캐릭터위에 뜨게 하자
                /*if (UTextBlock* WaveIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Character Level"))))
                {
                    WaveIndexText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d"), CurrentWaveIndex + 1)));
                }*/
            }
        }
    }
}
