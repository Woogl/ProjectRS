// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RsAnimNotifyBase.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsAnimNotifyState_WeaponTrace.generated.h"

class ARsWeapon;
/**
 * 
 */
UCLASS(Abstract)
class RS_API URsAnimNotifyState_WeaponTrace : public URsAnimNotifyStateBase
{
	GENERATED_BODY()
	
public:
	URsAnimNotifyState_WeaponTrace();
	
	UPROPERTY(EditAnywhere, meta=(Categories="AnimNotify"))
	FGameplayTag EventTag;
	
	UPROPERTY(EditAnywhere)
	FName WeaponID;
	
	UPROPERTY(EditAnywhere)
	FRsTargetingFilter Filter;

	UPROPERTY(EditAnywhere)
	FRsTargetingSorter Sorter;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	ARsWeapon* FindWeaponActor(USkeletalMeshComponent* OwnerMeshComp) const;
	
	TArray<AActor*> FindTargets(USkeletalMeshComponent* OwnerMeshComp);
	void SendHitEvent(USkeletalMeshComponent* OwnerMeshComp, TArray<AActor*> Targets);
	
	struct FWeaponTraceRuntimeData
	{
		TWeakObjectPtr<ARsWeapon> Weapon;
		FTransform LastTransform;
		TArray<TWeakObjectPtr<AActor>> HitActors;
	};
	
	TMap<TWeakObjectPtr<USkeletalMeshComponent>, FWeaponTraceRuntimeData> RuntimeDataMap;
};
