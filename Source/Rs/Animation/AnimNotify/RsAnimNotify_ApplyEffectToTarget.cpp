// Copyright 2024 Team BH.


#include "RsAnimNotify_ApplyEffectToTarget.h"

#include "Rs/Battle/RsBattleLibrary.h"

void URsAnimNotify_ApplyEffectToTarget::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	for (AActor* ResultActor : ResultActors)
	{
		URsBattleLibrary::ApplyEffectCoefficient(MeshComp->GetOwner(), ResultActor, EffectCoefficient);
	}
}
