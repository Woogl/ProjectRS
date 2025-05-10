// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility_Attack.h"
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
	static FGameplayEffectSpecHandle MakeEffectSpecCoefficient(UAbilitySystemComponent* SourceASC, const FRsEffectCoefficient& EffectCoefficient);
	
	UFUNCTION(BlueprintCallable, Category = "RS Battle Library")
	static void ApplyDamageContext(const AActor* Source, const AActor* Target, FRsDamageContext& DamageContext);
	
	UFUNCTION(BlueprintCallable, Category = "RS Battle Library")
	static FActiveGameplayEffectHandle ApplyEffectCoefficient(const AActor* Source, const AActor* Target, const FRsEffectCoefficient& EffectCoefficient);

	UFUNCTION(BlueprintCallable, Category = "RS Battle Library")
	static FActiveGameplayEffectHandle ApplyEffectSpecCoefficient(const AActor* Source, const AActor* Target, const FGameplayEffectSpecHandle& EffectHandle);
	
	UFUNCTION(BlueprintPure, Category = "RS Battle Library")
	static bool IsCriticalHitEffect(FGameplayEffectContextHandle EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "RS Battle Library", meta = (DefaultToSelf = "Target"))
	static bool IsDead(const AActor* Target);
};
