// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "RsAnimNotifyBase.generated.h"

class URsTriggerConditionBase;
class UGameplayAbility;
class UAbilitySystemComponent;
class URsConditionBase;
/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyBase : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Instanced, EditAnywhere, BlueprintReadWrite)
	TObjectPtr<URsConditionBase> Condition;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	TWeakObjectPtr<UAbilitySystemComponent> OwnerASC;
	TWeakObjectPtr<UGameplayAbility> CurrentAbility;
};

/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyStateBase : public UAnimNotifyState
{
	GENERATED_BODY()
	
protected:
	// Check condition every tick.
	UPROPERTY(Instanced, EditAnywhere)
	TObjectPtr<URsConditionBase> Condition;
	
	// Wait condition triggered.
	UPROPERTY(Instanced, EditAnywhere)
	TObjectPtr<URsTriggerConditionBase> Trigger;
	
	UPROPERTY(EditAnywhere)
	bool bShouldOnlyTriggerOnce = true;

public:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UFUNCTION()
	virtual void HandleConditionSatisfied();
	
	TWeakObjectPtr<UAbilitySystemComponent> OwnerASC;
	TWeakObjectPtr<UGameplayAbility> CurrentAbility;
	bool bHasTriggered = false;
	float CachedTotalDuration = 0.f;
	
#if WITH_EDITOR
	UFUNCTION()
	TArray<FName> GetSocketNames() const { return SocketNames; }
#endif // WITH_EDITOR
	
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	TArray<FName> SocketNames;
#endif // WITH_EDITORONLY_DATA
};