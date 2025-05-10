// Copyright 2024 Team BH.


#include "RsAnimNotify_HitScan.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Effect/RsDamageDefinition.h"
#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/System/RsDeveloperSetting.h"

void URsAnimNotify_HitScan::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	for (AActor* Victim : ResultActors)
	{
		URsBattleLibrary::ApplyDamageContext(MeshComp->GetOwner(), Victim, DamageContext);
	}
}
