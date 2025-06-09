// Copyright 2024 Team BH.


#include "RsAnimNotify_HitScan.h"

#include "AbilitySystemBlueprintLibrary.h"

FString URsAnimNotify_HitScan::GetNotifyName_Implementation() const
{
	if (DamageEvent.IsValid())
	{
		FString EventTagString = DamageEvent.ToString();
		return EventTagString.Replace(TEXT("AnimNotify."), TEXT(""));
	}
	return Super::GetNotifyName_Implementation();
}

void URsAnimNotify_HitScan::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	for (AActor* ResultActor : Targets)
	{
		FGameplayEventData Payload;
		Payload.EventTag = DamageEvent;
		Payload.Instigator = MeshComp->GetOwner();
		Payload.Target = ResultActor;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), DamageEvent, Payload);
	}
}
