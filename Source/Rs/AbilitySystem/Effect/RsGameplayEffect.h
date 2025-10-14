// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "RsGameplayEffect.generated.h"

class URsModifierDataEffectComponent;

/**
 * Categorized gameplay effect for designer.
 */
UCLASS(Abstract)
class RS_API URsGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	URsGameplayEffect();

#if WITH_EDITOR
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;

private:
	void SetModifiersFromAsset(const URsModifierDataEffectComponent* ModifierDataEffectComp);
	void SetModifiersFromTable(const URsModifierDataEffectComponent* ModifierDataEffectComp);
#endif // WITH_EDITOR
};
