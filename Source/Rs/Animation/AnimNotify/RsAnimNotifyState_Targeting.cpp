// Copyright 2024 Team BH.


#include "RsAnimNotifyState_Targeting.h"

#include "Rs/Targeting/RsTargetingLibrary.h"

URsAnimNotifyState_Targeting::URsAnimNotifyState_Targeting()
{
	bIsNativeBranchingPoint = true;
	
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = true;
#endif // WITH_EDITORONLY_DATA
}

void URsAnimNotifyState_Targeting::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp || !MeshComp->GetOwner())
	{
		return;
	}

	Targets.Reset();
	FTransform SourceTransform = SocketName.IsValid() ? MeshComp->GetSocketTransform(SocketName) : MeshComp->GetComponentTransform();
	SourceTransform.SetLocation(SourceTransform.GetLocation() + MeshComp->GetComponentTransform().TransformVector(PositionOffset));
	SourceTransform.SetRotation(RotationOffset.Quaternion() * SourceTransform.GetRotation());

	FRsTargetingCollision Collision(CollisionObjectTypes, ShapeType, HalfExtent, Radius, HalfHeight);
	FRsTargetingFilter Filter(bIncludeSelf, bIncludeFriendlyTeam, bIncludeHostileTeam, MaxTargetCount, TargetRequirements, TArray<AActor*>());
	FRsTargetingSorter Sorter(bSortByDistance);
	
	TArray<AActor*> OutActors;
	if (URsTargetingLibrary::PerformTargeting(MeshComp->GetOwner(), SourceTransform.GetLocation(), SourceTransform.Rotator(), Collision, Filter, Sorter, OutActors))
	{
		Targets = OutActors;
	}

#if WITH_EDITOR
	SocketNames = MeshComp->GetAllSocketNames();
#endif // WITH_EDITOR
}

void URsAnimNotifyState_Targeting::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
}
