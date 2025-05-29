// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectUIData.h"
#include "RsGameplayEffectUIDataComponent.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsGameplayEffectUIDataComponent : public UGameplayEffectUIData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FText Description;
};
