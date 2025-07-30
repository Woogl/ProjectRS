// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Rs/AbilitySystem/Effect/RsEffectCoefficient.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsBattleLibrary.generated.h"

class ARsCharacterBase;
struct FGameplayEffectContextHandle;
struct FGameplayEffectSpecHandle;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class RS_API URsBattleLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "RS Battle Library")
	static FGameplayEffectSpecHandle MakeEffectSpecCoefficient(UAbilitySystemComponent* SourceASC, const FRsEffectCoefficient& EffectCoefficient, FGameplayEffectContextHandle EffectContext);
	
	UFUNCTION(BlueprintCallable, Category = "RS Battle Library")
	static FActiveGameplayEffectHandle ApplyEffectCoefficient(const AActor* Source, const AActor* Target, const FRsEffectCoefficient& EffectCoefficient);

	UFUNCTION(BlueprintCallable, Category = "RS Battle Library")
	static FActiveGameplayEffectHandle ApplyEffectSpecCoefficient(const AActor* Source, const AActor* Target, const FGameplayEffectSpecHandle& EffectHandle);
	
	UFUNCTION(BlueprintPure, Category = "RS Battle Library")
	static bool IsCriticalHitEffect(FGameplayEffectContextHandle EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "RS Battle Library")
	static bool IsDotDamageEffect(FGameplayEffectContextHandle EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "RS Battle Library", meta = (DefaultToSelf = "Target"))
	static bool IsDead(const AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "RS Battle Library")
	static AActor* GetLockOnTarget(APawn* Character);
};
