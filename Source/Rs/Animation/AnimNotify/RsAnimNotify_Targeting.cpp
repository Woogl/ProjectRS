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

void URsAnimNotify_Targeting::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp == nullptr)
	{
		return;
	}
	
	UWorld* World = MeshComp->GetWorld();
	if (World == nullptr)
	{
		return;
	}

	ResultActors.Reset();
	
	FTransform SourceTransform = SocketName.IsValid() ? MeshComp->GetSocketTransform(SocketName) : MeshComp->GetComponentTransform();
	SourceTransform.SetLocation(SourceTransform.GetLocation() + MeshComp->GetComponentTransform().TransformVector(PositionOffset));
	SourceTransform.SetRotation(RotationOffset.Quaternion() * SourceTransform.GetRotation());

	FRsTargetingCollision Collision(CollisionObjectTypes, ShapeType, HalfExtent, Radius, HalfHeight);
	FRsTargetingFilter Filter(bIncludeSelf, bIncludeFriendlyTeam, bIncludeHostileTeam, MaxTargetCount, TargetRequirements, TArray<AActor*>());
	FRsTargetingSorter Sorter(bSortByDistance);
	
	TArray<AActor*> OutActors;
	if (MeshComp->GetOwner())
	{
		if (URsTargetingLibrary::PerformTargeting(MeshComp->GetOwner(), SourceTransform.GetLocation(), SourceTransform.GetRotation().Rotator(), Collision, Filter, Sorter, OutActors))
		{
			ResultActors = OutActors;
		
			// For GA's damage event compatibility
			for (AActor* ResultActor : ResultActors)
			{
				FGameplayEventData Payload;
				Payload.EventTag = EventTag;
				Payload.Instigator = MeshComp->GetOwner();
				Payload.Target = ResultActor;
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(ResultActor, EventTag, Payload);
			}
		}
	}

#if WITH_EDITOR
	SocketNames = MeshComp->GetAllSocketNames();
#endif // WITH_EDITOR
}
