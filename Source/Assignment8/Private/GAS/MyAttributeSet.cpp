#include "GAS/MyAttributeSet.h"
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


}

