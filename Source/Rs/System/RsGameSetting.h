// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "GameplayEffect.h"
#include "RsGameSetting.generated.h"

/**
 * Golbal game setting datas for Project RS
 * Datas in this class is always loaded in memory.
 */
USTRUCT(BlueprintType)
struct FRsEffectTagData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag EffectTag;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> EffectClass;

	bool operator==(const FRsEffectTagData& Other) const { return this->EffectTag.MatchesTagExact(Other.EffectTag); }

	FRsEffectTagData() : EffectTag(FGameplayTag::EmptyTag), EffectClass(nullptr) {}
	FRsEffectTagData(const FGameplayTag InEffectTag) : EffectTag(InEffectTag), EffectClass(nullptr) {}
	FRsEffectTagData(const FGameplayTag InEffectTag, const TSubclassOf<UGameplayEffect>& InEffectClass) : EffectTag(InEffectTag), EffectClass(InEffectClass) {}
};

UCLASS()
class RS_API URsGameSetting : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	static URsGameSetting* Get();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | Battle")
	FGameplayTag HealthDamageCoefficientTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | Battle")
	FGameplayTag StaggerDamageCoefficientTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | Battle")
	FGameplayTag DeathAbilityTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | Battle")
	FGameplayTag GroggyAbilityTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tag for Native Code | Battle")
	FGameplayTag LockOnAbilityTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (Categories = "Effect.Damage"))
	TArray<FRsEffectTagData> DamageEffectTagTable;
};
