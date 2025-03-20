// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RsBattleLibrary.generated.h"

class ARsCharacterBase;
struct FGameplayEffectContextHandle;
struct FGameplayEffectSpecHandle;
class UGameplayEffect;
class UTargetingPreset;
/**
 * 
 */
USTRUCT(BlueprintType)
struct FRsDamageEffectParams
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,  meta = (Categories = "Effect.Damage"))
	FGameplayTag EffectTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float EffectLevel;

	bool IsValid() const { return EffectTag.IsValid(); }

	FRsDamageEffectParams() : EffectTag(FGameplayTag::EmptyTag), EffectLevel(0.0f) {}
	FRsDamageEffectParams(const FGameplayTag EffectTag) : EffectTag(EffectTag), EffectLevel(0.0f) {}
	FRsDamageEffectParams(const FGameplayTag EffectTag, const float EffectLevel) : EffectTag(EffectTag), EffectLevel(EffectLevel) {}
	FRsDamageEffectParams(const FRsDamageEffectParams& Other) { *this = Other; }
};

USTRUCT()
struct FRsDamageEffectData
{
	GENERATED_BODY()

	uint32 Order;

	TSubclassOf<UGameplayEffect> EffectClass;

	float EffectLevel;

	FRsDamageEffectData() : Order(0), EffectClass(nullptr), EffectLevel(0.0f) {}
	FRsDamageEffectData(const uint32 Order, const TSubclassOf<UGameplayEffect>& EffectClass, const float EffectLevel) : Order(Order), EffectClass(EffectClass), EffectLevel(EffectLevel) {}
};

UCLASS()
class RS_API URsBattleLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "RS Battle Library")
	static bool ExecuteTargeting(AActor* SourceActor, const UTargetingPreset* TargetingPreset, TArray<AActor*>& ResultActors);

	UFUNCTION(BlueprintCallable, Category = "RS Battle Library")
	static TArray<FGameplayEffectSpecHandle> MakeDamageEffectSpecs(const AActor* SourceActor, const TArray<FRsDamageEffectParams>& Params);

	UFUNCTION(BlueprintCallable, Category = "RS Battle Library")
	static bool ApplyDamageEffectSpecs(const AActor* SourceActor, const AActor* TargetActor, const TArray<FGameplayEffectSpecHandle>& Handles);

	static TArray<FRsDamageEffectData> GetEffectDataList(const TArray<FRsDamageEffectParams>& Params);
	
	UFUNCTION(BlueprintPure, Category = "RS Battle Library")
	static bool IsCriticalHitEffect(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "RS Battle Library", meta = (DefaultToSelf = "Character"))
	static bool IsDead(const ARsCharacterBase* Character);
};
