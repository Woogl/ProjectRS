// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Rs/AbilitySystem/Effect/RsEffectCoefficient.h"
#include "RsBattleLibrary.generated.h"

struct FGameplayEffectContextHandle;
struct FGameplayEffectSpecHandle;
class UGameplayEffect;
class UTargetingPreset;
/**
 * 
 */
UCLASS()
class RS_API URsBattleLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "RS Battle Library")
	static bool ExecuteTargeting(AActor* SourceActor, const UTargetingPreset* TargetingPreset, TArray<AActor*>& ResultActors);

	UFUNCTION(BlueprintCallable, Category = "RS Battle Library")
	static void SortDamageEffectsByOrder(FRsDamageEventContext& DamageContexts);
	
	UFUNCTION(BlueprintCallable, Category = "RS Battle Library")
	static FActiveGameplayEffectHandle ApplyEffectCoefficient(const AActor* SourceActor, const AActor* TargetActor, FRsEffectCoefficient EffectCoefficient);

	UFUNCTION(BlueprintCallable, Category = "RS Battle Library")
	static FActiveGameplayEffectHandle ApplyEffectSpecCoefficient(const AActor* SourceActor, const AActor* TargetActor, const FGameplayEffectSpecHandle& EffectHandle);
	
	UFUNCTION(BlueprintPure, Category = "RS Battle Library")
	static bool IsCriticalHitEffect(FGameplayEffectContextHandle EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "RS Battle Library", meta = (DefaultToSelf = "Target"))
	static bool IsDead(const AActor* Target);
};
