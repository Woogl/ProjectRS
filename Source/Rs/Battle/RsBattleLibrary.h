// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Rs/AbilitySystem/Effect/RsEffectCoefficient.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsBattleLibrary.generated.h"

enum class ERsLinkSkillType : uint8;
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

	UFUNCTION(BlueprintPure, Category = "RS Battle Library")
	static AActor* GetLockOnTarget(APawn* Character);

	/** Link skill */
	UFUNCTION(BlueprintPure, Category = "RS Battle Library", meta=(WorldContext="WorldContextObject"))
	static void GetLinkSkillInfo(UObject* WorldContextObject, ARsCharacterBase*& LinkSkillTarget, ERsLinkSkillType& LinkSkillType, int32& AvailableCount);
	
	UFUNCTION(BlueprintCallable, Category = "RS Battle Library", meta=(WorldContext="WorldContextObject"))
	static void SetLinkSkillTarget(UObject* WorldContextObject, ARsCharacterBase* LinkSkillTarget, ERsLinkSkillType Type);

	UFUNCTION(BlueprintCallable, Category = "RS Battle Library", meta=(WorldContext="WorldContextObject"))
	static void DecrementLinkSkillTarget(UObject* WorldContextObject, ARsCharacterBase* LinkSkillTarget, ERsLinkSkillType Type);
};
