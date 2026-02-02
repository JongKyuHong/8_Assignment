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
#include "Misc/OutputDeviceNull.h"

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

// 게임 인스턴스에 스코어 추가
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


// 레벨 스타트, 한개의 레벨에는 3개의 웨이브가 있고 총 3레벨 까지 있음
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

                            float ScaleValue = MyGameInstance->CharacterScale;
                            if (ScaleValue > 0.0f)
                            {
                                PlayerPawn->SetActorScale3D(FVector(ScaleValue));
                            }
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
    if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
    {
        GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AMyGameState::EndWave);
    }
}

void AMyGameState::StartWave()
{
    SpawnedCoinCount = 0;
    CollectedCoinCount = 0;

    TArray<AActor*> FoundVolumes;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

    const int32 ItemToSpawn = (CurrentWaveIndex + 10) * ((CurrentLevelIndex + 1) * 3);

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

    FString WaveMessage = TEXT("");
    if (CurrentWaveIndex == 1)
    {
        WaveMessage = TEXT("Slow Potion이 추가되었습니다.");
    }
    else if (CurrentWaveIndex == 2)
    {
        WaveMessage = TEXT("Poision Potion이 추가되었습니다.");
    }

    if (!WaveMessage.IsEmpty())
    {
        if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
        {
            if (AMyPlayerController* MyPC = Cast<AMyPlayerController>(PC))
            {
                if (UUserWidget* HUDWidget = MyPC->GetHUDWidget())
                {
                    FOutputDeviceNull ar;
                    FString Command = FString::Printf(TEXT("ShowNotification \"%s\""), *WaveMessage);
                    HUDWidget->CallFunctionByNameWithArguments(*Command, ar, NULL, true);
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

    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (AMyPlayerController* MyPC = Cast<AMyPlayerController>(PC))
        {
            MyPC->PlayWaveStartUI();
        }
    }
}

// 증강을 고르는 UI띄우기
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
    if (CurrentLevelIndex >= MaxLevels) return;
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GameInstance);
        if (MyGameInstance)
        {
            AddScore(Score);
            CurrentLevelIndex++;

            if (CurrentLevelIndex >= MaxLevels)
            {
                OnGameOver();
                return;
            }

            MyGameInstance->CurrentLevelIndex = CurrentLevelIndex;

            // 게임 인스턴스에 정보 저장
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
                            //MyGameInstance->MoveSpeed = AttributeSet->GetMoveSpeed();
                            MyGameInstance->MoveSpeed = ASC->GetNumericAttributeBase(UMyAttributeSet::GetMoveSpeedAttribute());
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

                if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
                {
                    LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d - %d"), CurrentLevelIndex + 1, CurrentWaveIndex+1)));
                }
            }
        }
    }
}
