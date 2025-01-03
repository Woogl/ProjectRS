// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RsBattleLibrary.generated.h"

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
	static TArray<AActor*> PerformTargeting(AActor* SourceActor, const UTargetingPreset* TargetingPreset);

	UFUNCTION(BlueprintCallable, Category = "RS Battle Library")
	static void ApplyDamageEffect(AActor* SourceActor, AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);
};
