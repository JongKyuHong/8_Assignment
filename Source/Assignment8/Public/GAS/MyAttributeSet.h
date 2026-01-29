#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MyAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class ASSIGNMENT8_API UMyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UMyAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, MoveSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData JumpZVelocity;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, JumpZVelocity)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData CharacterScale;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, CharacterScale)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	FGameplayAttributeData ExpGainRate;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, ExpGainRate)

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};
