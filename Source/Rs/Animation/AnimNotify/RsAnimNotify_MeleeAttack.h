// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsAnimNotify_Targeting.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility_Attack.h"
#include "RsAnimNotify_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsAnimNotify_MeleeAttack : public URsAnimNotify_Targeting
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage")
	FRsDamageEventContext DamageContext;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
