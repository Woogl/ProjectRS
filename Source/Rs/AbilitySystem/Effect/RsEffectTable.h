// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsEffectTable.generated.h"

class UGameplayEffect;
/**
 * 
 */
USTRUCT()
struct FRsEffectTableRowBase : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> EffectClass;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif // WITH_EDITOR
};

USTRUCT(BlueprintType)
struct FRsEffectTableRow : public FRsEffectTableRowBase
{
	GENERATED_BODY()

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif // WITH_EDITOR

	/** Polymorphic parameters */
	UPROPERTY(EditDefaultsOnly)
	FName Key1 = NAME_None;

	UPROPERTY(EditDefaultsOnly)
	FString Value1 = FString();

	UPROPERTY(EditDefaultsOnly)
	FName Key2 = NAME_None;

	UPROPERTY(EditDefaultsOnly)
	FString Value2 = FString();

	UPROPERTY(EditDefaultsOnly)
	FName Key3 = NAME_None;

	UPROPERTY(EditDefaultsOnly)
	FString Value3 = FString();

	UPROPERTY(EditDefaultsOnly)
	FName Key4 = NAME_None;

	UPROPERTY(EditDefaultsOnly)
	FString Value4 = FString();

	UPROPERTY(EditDefaultsOnly)
	FName Key5 = NAME_None;

	UPROPERTY(EditDefaultsOnly)
	FString Value5 = FString();

	UPROPERTY(EditDefaultsOnly)
	FName Key6 = NAME_None;

	UPROPERTY(EditDefaultsOnly)
	FString Value6 = FString();

	UPROPERTY(EditDefaultsOnly)
	FName Key7 = NAME_None;

	UPROPERTY(EditDefaultsOnly)
	FString Value7 = FString();

	UPROPERTY(EditDefaultsOnly)
	FName Key8 = NAME_None;
	
	UPROPERTY(EditDefaultsOnly)
	FString Value8 = FString();

	UPROPERTY(EditDefaultsOnly)
	FName Key9 = NAME_None;
	
	UPROPERTY(EditDefaultsOnly)
	FString Value9 = FString();

	/** Time (Optional) */
	UPROPERTY(EditDefaultsOnly)
	float Duration = 0.f;

	/** UI Data (Optional) */
	UPROPERTY(EditDefaultsOnly)
	FText EffectName = FText::GetEmpty();
	
	UPROPERTY(EditDefaultsOnly)
	FText Description = FText::GetEmpty();

	template <typename T>
	T FindValue(FName Key, bool bWarnIfNotFound) const;

private:
	FString FindValueInternal(FName Key, bool bWarnIfNotFound) const;
};

template <typename T>
T FRsEffectTableRow::FindValue(FName Key, bool bWarnIfNotFound) const
{
	FString StringValue = FindValueInternal(Key, bWarnIfNotFound);
	if constexpr (std::is_same_v<T, FString>)
	{
		return StringValue;
	}
	else if constexpr (std::is_same_v<T, FName>)
	{
		return FName(*StringValue);
	}
	else if constexpr (std::is_same_v<T, int32>)
	{
		return FCString::Atoi(*StringValue);
	}
	else if constexpr (std::is_same_v<T, float>)
	{
		return FCString::Atof(*StringValue);
	}
	else if constexpr (std::is_same_v<T, bool>)
	{
		return StringValue.ToBool();
	}
	else if constexpr (std::is_same_v<T, FGameplayTag>)
	{
		return FGameplayTag::RequestGameplayTag(FName(*StringValue));
	}
	else if constexpr (TIsEnum<T>::Value)
	{
		UEnum* EnumPtr = StaticEnum<T>();
		int64 EnumValue = EnumPtr->GetValueByNameString(StringValue);
		return static_cast<T>(EnumValue);
	}
	else
	{
		static_assert("FRsEffectTableRow::FindValue: Unsupported type T.");
		return T{};
	}
}

/**
 * 
 */
USTRUCT(BlueprintType)
struct FRsDamageTableRow : public FRsEffectTableRowBase
{
	GENERATED_BODY()
	
	/** Magnitude */
	UPROPERTY(EditAnywhere)
	FString HealthDamageExpression;

	UPROPERTY(EditAnywhere)
	FString StaggerDamageExpression;

	/** Can apply */
	UPROPERTY(EditDefaultsOnly)
	bool InvinciblePierce = false;

	UPROPERTY(EditDefaultsOnly)
	bool SuperArmorPierce = false;
	
	UPROPERTY(EditDefaultsOnly)
	bool GuardPierce = false;

	/** Hit reaction */
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Ability.HitReaction"))
	FGameplayTag HitReaction;

	/** Hit stop */
	UPROPERTY(EditDefaultsOnly)
	float SourceHitStopTime = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float TargetHitStopTime = 0.f;

	/** Advantage to source */
	UPROPERTY(EditDefaultsOnly)
	float ManaGain = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float UltimateGain = 0.f;
};
