// Copyright 2024 Team BH.


#include "RsAnimNotify_BuffToSelf.h"

#include "Rs/AbilitySystem/Effect/RsBuffEffect.h"
#include "Rs/Battle/RsBattleLibrary.h"

URsAnimNotify_BuffToSelf::URsAnimNotify_BuffToSelf()
{
	bIsNativeBranchingPoint = true;
}

void URsAnimNotify_BuffToSelf::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	URsBattleLibrary::ApplyEffectCoefficient(MeshComp->GetOwner(), MeshComp->GetOwner(), BuffCoefficient.ToRsEffectCoefficient(Duration));
}
