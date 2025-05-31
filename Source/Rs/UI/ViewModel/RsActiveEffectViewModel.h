// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "RsViewModelBase.h"
#include "RsActiveEffectViewModel.generated.h"

class URsAbilitySystemComponent;
class URsGameplayEffectUIDataComponent;
/**
 * 
 */

UENUM(BlueprintType)
enum class ERsActiveEffectCategory : uint8
{
	DotDamage,
	Debuff,
	Buff,
	Others,
};


UCLASS()
class RS_API URsActiveEffectViewModel : public URsViewModelBase, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static URsActiveEffectViewModel* CreateRsActiveEffectViewModel(FActiveGameplayEffectHandle EffectHandle);
	
	UFUNCTION(FieldNotify, BlueprintPure)
	float GetEffectProgress() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetStackDataText() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	UTexture2D* GetIcon() const;

	UFUNCTION(FieldNotify, BlueprintPure)
	FText GetDescription() const;
	
	UFUNCTION(FieldNotify, BlueprintPure)
	ERsActiveEffectCategory GetCategory() const;
	
protected:
	static const URsGameplayEffectUIDataComponent* FindRsUIData(const FActiveGameplayEffect& Effect);

	virtual void Initialize() override;
	virtual void Deinitialize() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnEffectAdded(const FActiveGameplayEffect& Effect);
	virtual void OnEffectRenewed(FActiveGameplayEffectHandle EffectHandle, float NewStartTime, float NewDuration);
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(URsAbilityViewModel, STATGROUP_Tickables); };
protected:
	UPROPERTY()
	TWeakObjectPtr<URsAbilitySystemComponent> CachedASC;
	
	const FActiveGameplayEffect* CachedModel;
};
