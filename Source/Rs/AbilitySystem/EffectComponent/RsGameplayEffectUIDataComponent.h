// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectUIData.h"
#include "RsGameplayEffectUIDataComponent.generated.h"

UENUM(BlueprintType)
enum class ERsEffectCategory : uint8
{
	DotDamage,
	Debuff,
	Buff,
	Others,
	Invalid UMETA(HIDDEN),
};

UCLASS()
class RS_API URsGameplayEffectUIDataComponent : public UGameplayEffectUIData
{
	GENERATED_BODY()
	
public:

	bool operator==(const URsGameplayEffectUIDataComponent& other) const { return (this->Icon == other.Icon) && (this->Category == other.Category); };
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FText Description;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	ERsEffectCategory Category;
};
