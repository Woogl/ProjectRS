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

	UFUNCTION(BlueprintPure, Category = "RS Battle Library", meta = (DefaultToSelf = "Target"))
	static bool IsDead(const AActor* Target);

	UFUNCTION(BlueprintPure, Category = "RS Battle Library")
	static AActor* GetLockOnTarget(APawn* Character);
};
