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
	FText Title = FText::GetEmpty();
	
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
	static_assert(!std::is_same_v<T, T>, "FRsEffectTableRow::FindValue: Unsupported type. Add an explicit specialization.");
	return T{};
}

template <>
FString FRsEffectTableRow::FindValue<FString>(FName Key, bool bWarnIfNotFound) const;

template <>
FName FRsEffectTableRow::FindValue<FName>(FName Key, bool bWarnIfNotFound) const;

template <>
int32 FRsEffectTableRow::FindValue<int32>(FName Key, bool bWarnIfNotFound) const;

template <>
float FRsEffectTableRow::FindValue<float>(FName Key, bool bWarnIfNotFound) const;

template <>
bool FRsEffectTableRow::FindValue<bool>(FName Key, bool bWarnIfNotFound) const;

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
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0", ClampMax = "3"))
	int32 InvinciblePierce = 0;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0", ClampMax = "3"))
	int32 SuperArmorPierce = 0;

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

	/** Additional Effect */
	UPROPERTY(EditDefaultsOnly)
	FName AdditionalSourceEffect = NAME_None;

	UPROPERTY(EditDefaultsOnly)
	FName AdditionalTargetEffect = NAME_None;
};
