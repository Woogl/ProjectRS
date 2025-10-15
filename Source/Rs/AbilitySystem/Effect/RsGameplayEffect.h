// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "RsGameplayEffect.generated.h"

class URsModifierDataEffectComponent;

/**
 * Categorized gameplay effect for designer.
 * It supports automatic setting of modifiers using RsModifierDataEffectComponent.
 */
UCLASS(Abstract)
class RS_API URsGameplayEffect : public UGameplayEffect
{
	GENERATED_BODY()

public:
	URsGameplayEffect();

#if WITH_EDITOR
	virtual void PreSave(FObjectPreSaveContext SaveContext) override;
	virtual bool CanEditChange(const FEditPropertyChain& PropertyChain) const override;

private:
	const URsModifierDataEffectComponent* FindModifierDataComponent() const;
	void SetModifiersFromAsset(const URsModifierDataEffectComponent* ModifierDataComp);
	void SetModifiersFromTable(const URsModifierDataEffectComponent* ModifierDataComp);
#endif // WITH_EDITOR
};
