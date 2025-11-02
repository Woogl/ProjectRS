// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "RsGameplayEffect.generated.h"

class URsDamageEffectComponent;
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

	// TODO: Refactor functions.
	void AddModifier(const FGameplayModifierInfo& Modifier);
	void AddExecution(const FGameplayEffectExecutionDefinition& Execution);

private:
	/** RsModifierDataEffectComponent setup */
	void SetModifiersFromAsset(const URsModifierDataEffectComponent* ModifierDataComp);
	void SetModifiersFromTable(const URsModifierDataEffectComponent* ModifierDataComp);

	/** RsDamageDataEffectComponent setup */
	void SetModifiersFromAsset(const URsDamageEffectComponent* DamageDataComp);
	void SetModifiersFromTable(const URsDamageEffectComponent* DamageDataComp);
	void SetDamageExecutions(const URsDamageEffectComponent* DamageDataComp);
	void SetGameplayCues(const URsDamageEffectComponent* DamageDataComp);
#endif // WITH_EDITOR
};
