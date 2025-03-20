// Copyright 2024 Team BH.


#include "RsAnimNotify_MeleeAttack.h"

#include "Rs/Battle/RsBattleLibrary.h"

void URsAnimNotify_MeleeAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp->GetOwner())
	{
		for (AActor* Victim : ResultActors)
	 	{
	 		//URsBattleLibrary::ApplyDamageEffect(MeshComp->GetOwner(), Victim, DamageContext.DamageEffectClass, DamageContext.DamageEffectTags);
	 	}
	}
}
