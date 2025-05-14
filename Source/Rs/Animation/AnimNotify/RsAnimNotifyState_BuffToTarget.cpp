// Copyright 2024 Team BH.


#include "RsAnimNotifyState_BuffToTarget.h"

#include "Rs/Battle/RsBattleLibrary.h"

void URsAnimNotifyState_BuffToTarget::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	for (TWeakObjectPtr<AActor> Target : Targets)
	{
		if (Target.IsValid())
		{
			URsBattleLibrary::ApplyEffectCoefficient(MeshComp->GetOwner(), Target.Get(), BuffCoefficient.ToRsEffectCoefficient(TotalDuration));
		}
	}
}
