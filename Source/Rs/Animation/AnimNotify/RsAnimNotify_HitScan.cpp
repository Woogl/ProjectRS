// Copyright 2024 Team BH.


#include "RsAnimNotify_HitScan.h"

#include "Rs/Battle/RsBattleLibrary.h"

FString URsAnimNotify_HitScan::GetNotifyName_Implementation() const
{
	if (DamageContext.DamageEventTag.IsValid())
	{
		FString EventTagString = DamageContext.DamageEventTag.ToString();
		return EventTagString.Replace(TEXT("AnimNotify."), TEXT(""));
	}
	return Super::GetNotifyName_Implementation();
}

void URsAnimNotify_HitScan::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	for (AActor* Target : Targets)
	{
		URsBattleLibrary::ApplyDamageContext(MeshComp->GetOwner(), Target, DamageContext);
	}
}
