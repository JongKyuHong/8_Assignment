#include "GAS/MyAttributeSet.h"
#include "GameState/MyGameState.h"
#include "GameplayEffect.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayEffectExtension.h"

UMyAttributeSet::UMyAttributeSet()
{
	InitHealth(100.0f);
	InitMaxHealth(100.0f);
	InitMoveSpeed(400.0f);
	InitJumpZVelocity(400.0f);
	InitCharacterScale(1.0f);
	InitExpGainRate(1.0f);
	InitLevel(1.0f);
	InitExp(0.0f);
	InitMaxExp(100.0f);
}

void UMyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayAttribute ModifiedAttribute = Data.EvaluatedData.Attribute;
	AActor* TargetActor = Data.Target.GetAvatarActor();
	ACharacter* TargetChar = Cast<ACharacter>(TargetActor);

	if (!TargetActor) return;

	if (ModifiedAttribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (ModifiedAttribute == GetMoveSpeedAttribute() && TargetChar)
	{
		TargetChar->GetCharacterMovement()->MaxWalkSpeed = GetMoveSpeed();
	}
	else if (ModifiedAttribute == GetJumpZVelocityAttribute() && TargetChar)
	{
		TargetChar->GetCharacterMovement()->JumpZVelocity = GetJumpZVelocity();
	}
	else if (ModifiedAttribute == GetCharacterScaleAttribute())
	{
		TargetActor->SetActorScale3D(FVector(GetCharacterScale()));
	}
	else if (ModifiedAttribute == GetExpAttribute())
	{
		float CurrentExp = GetExp();
		float MaxExpValue = GetMaxExp();

		if (MaxExpValue <= 0.f) return;

		if (CurrentExp >= MaxExpValue)
		{
			float RemainingExp = GetExp() - GetMaxExp();
			float NewLevel = GetLevel() + 1.0f;
			SetLevel(NewLevel);
			SetMaxExp(NewLevel * 100.0f);
			SetExp(RemainingExp);
			AMyGameState* MyGameState = GetWorld() ? GetWorld()->GetGameState<AMyGameState>() : nullptr;
			if (MyGameState)
			{
				MyGameState->ShowPerkMenu();
			}
			UE_LOG(LogTemp, Warning, TEXT("Level Up! Current Level: %d"), (int32)NewLevel);
		}

		SetExp(FMath::Max(GetExp(), 0.0f));
	}
}

