// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RsBattleLibrary.generated.h"

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
	static void ApplyDamageEffect(AActor* SourceActor, AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable, Category = "RS Battle Library")
	static void ApplyDamageEffectSpec(AActor* SourceActor, AActor* TargetActor, const FGameplayEffectSpecHandle& EffectHandle);
};
