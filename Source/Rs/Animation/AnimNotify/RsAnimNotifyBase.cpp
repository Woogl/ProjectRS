// Copyright 2025 Team BH.


#include "RsAnimNotifyBase.h"

#include "Rs/Condition/RsCondition.h"

void URsAnimNotifyBase::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	for (int32 i = 0; i < Conditions.Num(); i++)
	{
		if (!Conditions[i])
		{
			continue;
		}
		if (!Conditions[i]->IsSatisfied(Owner))
		{
			return;
		}
	}
	
	// Do something in child classes.
}
