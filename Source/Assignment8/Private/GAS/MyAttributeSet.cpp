#include "GAS/MyAttributeSet.h"
#include "Character/MyCharacter.h"
#include "GameState/MyGameState.h"
#include "GameplayEffect.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayEffectExtension.h"

UMyAttributeSet::UMyAttributeSet()
{
	InitMaxHealth(100.0f);
	InitMoveSpeed(600.0f);
	InitJumpZVelocity(300.0f);
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
	UE_LOG(LogTemp, Warning, TEXT("Modified Attribute Name: %s"), *ModifiedAttribute.GetName());
	AActor* TargetActor = Data.Target.GetAvatarActor();
	ACharacter* TargetChar = Cast<ACharacter>(TargetActor);

	if (!TargetActor) return;

	if (ModifiedAttribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (ModifiedAttribute == GetMaxHealthAttribute())
	{
		AMyCharacter* MyCharacter = Cast<AMyCharacter>(TargetActor);
		if (MyCharacter)
		{
			MyCharacter->UpdateOverheadBar();
		}
	}
	else if (ModifiedAttribute == GetMoveSpeedAttribute() && TargetChar)
	{
		if (TargetChar && TargetChar->GetCharacterMovement())
		{
			float FinalValue = GetMoveSpeed();
			TargetChar->GetCharacterMovement()->MaxWalkSpeed = FinalValue;

			UE_LOG(LogTemp, Warning, TEXT("Current MoveSpeed Attribute: %f"), FinalValue);
		}
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
		}

		SetExp(FMath::Max(GetExp(), 0.0f));
	}

}

void UMyAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMoveSpeedAttribute())
	{
		AActor* TargetActor = GetOwningAbilitySystemComponent()->GetAvatarActor();
		ACharacter* TargetChar = Cast<ACharacter>(TargetActor);

		if (TargetChar && TargetChar->GetCharacterMovement())
		{
			TargetChar->GetCharacterMovement()->MaxWalkSpeed = NewValue;
		}
	}
}