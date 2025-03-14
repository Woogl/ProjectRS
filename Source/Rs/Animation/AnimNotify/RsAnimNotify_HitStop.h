// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "RsAnimNotify_HitStop.generated.h"

class URsAbilitySystemComponent;
struct FGameplayEffectSpecHandle;
/**
 * 
 */
UCLASS()
class RS_API URsAnimNotify_HitStop : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AnimNotify")
	float Duration = 0.2f;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Debug")
	bool bEnableInEditorPreview = false;
#endif // WITH_EDITORONLY_DATA
	
private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UFUNCTION()
	void HandleDealDamage(UAbilitySystemComponent* TargetASC, FGameplayEffectSpecHandle DamageEffectHandle);

	TWeakObjectPtr<URsAbilitySystemComponent> RsAbilitySystemComponent;
	
#if WITH_EDITOR
	void ResumePlay();
#endif // WITH_EDITOR

#if WITH_EDITORONLY_DATA
	TWeakObjectPtr<USkeletalMeshComponent> CachedMeshComp;
	FTimerHandle Timer;
#endif // WITH_EDITORONLY_DATA
};
