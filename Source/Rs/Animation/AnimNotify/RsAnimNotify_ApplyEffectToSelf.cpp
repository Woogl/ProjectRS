// Copyright 2024 Team BH.


#include "RsAnimNotify_ApplyEffectToSelf.h"

#include "Rs/Battle/RsBattleLibrary.h"

URsAnimNotify_ApplyEffectToSelf::URsAnimNotify_ApplyEffectToSelf()
{
	bIsNativeBranchingPoint = true;
}

void URsAnimNotify_ApplyEffectToSelf::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	URsBattleLibrary::ApplyEffectCoefficient(MeshComp->GetOwner(), MeshComp->GetOwner(), EffectCoefficient);
}
