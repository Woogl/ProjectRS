// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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
	static void ApplyDamageEffect(const AActor* SourceActor, const AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable, Category = "RS Battle Library")
	static void ApplyDamageEffectSpec(const AActor* SourceActor, const AActor* TargetActor, const FGameplayEffectSpecHandle& EffectHandle);

	UFUNCTION(BlueprintPure, Category = "RS Battle Library")
	static bool IsCriticalHitEffect(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle);
};
