// Copyright 2024 Team BH.


#include "RsAnimNotifyState_BuffToSelf.h"

#include "Rs/Battle/RsBattleLibrary.h"

URsAnimNotifyState_BuffToSelf::URsAnimNotifyState_BuffToSelf()
{
	bIsNativeBranchingPoint = true;
}

void URsAnimNotifyState_BuffToSelf::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	URsBattleLibrary::ApplyEffectCoefficient(MeshComp->GetOwner(), MeshComp->GetOwner(), BuffCoefficient.ToRsEffectCoefficient(TotalDuration));
}
