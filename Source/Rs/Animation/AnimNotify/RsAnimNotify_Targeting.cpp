// Copyright 2024 Team BH.


#include "RsAnimNotify_Targeting.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

URsAnimNotify_Targeting::URsAnimNotify_Targeting()
{
	bIsNativeBranchingPoint = true;
	
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = true;
#endif // WITH_EDITORONLY_DATA
}

FString URsAnimNotify_Targeting::GetNotifyName_Implementation() const
{
	if (EventTag.IsValid())
	{
		FString EventTagString = EventTag.ToString();
		return EventTagString.Replace(TEXT("AnimNotify."), TEXT(""));
	}
	return Super::GetNotifyName_Implementation();
}

void URsAnimNotify_Targeting::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (!MeshComp)
	{
		return;
	}
	
	UWorld* World = MeshComp->GetWorld();
	if (!World)
	{
		return;
	}

	Targets.Reset();
	
	FTransform SourceTransform = SocketName.IsValid() ? MeshComp->GetSocketTransform(SocketName) : MeshComp->GetComponentTransform();
	TArray<AActor*> OutActors;
	if (MeshComp->GetOwner())
	{
		if (URsTargetingLibrary::PerformTargeting(MeshComp->GetOwner(), SourceTransform, Collision, Filter, Sorter, OutActors))
		{
			Targets = OutActors;
		
			// For GA's damage event compatibility
			for (AActor* ResultActor : Targets)
			{
				FGameplayEventData Payload;
				Payload.EventTag = EventTag;
				Payload.Instigator = MeshComp->GetOwner();
				Payload.Target = ResultActor;
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EventTag, Payload);
			}
		}
	}

#if WITH_EDITOR
	SocketNames = MeshComp->GetAllSocketNames();
#endif // WITH_EDITOR
}
