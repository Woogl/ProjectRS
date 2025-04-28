// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotify_Targeting.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility_Attack.h"
#include "RsAnimNotify_HitScan.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsAnimNotify_HitScan : public URsAnimNotify_Targeting
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	FRsDamageContext DamageContext;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
