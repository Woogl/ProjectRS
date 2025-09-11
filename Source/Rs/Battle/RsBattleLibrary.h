// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsBattleLibrary.generated.h"

enum class ERsLinkSkillType : uint8;
class ARsCharacterBase;
struct FGameplayEffectContextHandle;
/**
 * 
 */
UCLASS()
class RS_API URsBattleLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
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
