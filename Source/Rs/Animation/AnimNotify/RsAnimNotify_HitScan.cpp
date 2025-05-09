﻿// Copyright 2024 Team BH.


#include "RsAnimNotify_HitScan.h"

#include "Rs/Battle/RsBattleLibrary.h"

void URsAnimNotify_HitScan::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	for (AActor* Victim : ResultActors)
	{
		URsBattleLibrary::ApplyDamageContext(MeshComp->GetOwner(), Victim, DamageContext);
	}
}
