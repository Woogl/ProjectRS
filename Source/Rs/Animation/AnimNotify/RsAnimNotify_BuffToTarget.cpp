// Copyright 2024 Team BH.


#include "RsAnimNotify_BuffToTarget.h"

#include "Rs/Battle/RsBattleLibrary.h"

void URsAnimNotify_BuffToTarget::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	for (AActor* ResultActor : ResultActors)
	{
		URsBattleLibrary::ApplyEffectCoefficient(MeshComp->GetOwner(), ResultActor, BuffCoefficient.ToRsEffectCoefficient(Duration));
	}
}
