// Copyright 2024 Team BH.


#include "RsLockOnComponent.h"

#include "RsLockOnInterface.h"
#include "Components/WidgetComponent.h"

URsLockOnComponent::URsLockOnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void URsLockOnComponent::LockOn(AActor* TargetActor)
{
	if (IRsLockOnInterface* LockOnInterface = Cast<IRsLockOnInterface>(TargetActor))
	{
		if (LockOnInterface->Execute_IsLockableTarget(TargetActor) == false)
		{
			return;
		}
	}
	
	LockedOnTarget = TargetActor;

	if (LockedOnTarget.IsValid() && ReticleWidget != nullptr)
	{
		if (ReticleComponent == nullptr)
		{
			// Create new reticle widget.
			ReticleComponent = NewObject<UWidgetComponent>(TargetActor);
			ReticleComponent->SetWidgetClass(ReticleWidget);
			ReticleComponent->SetWidgetSpace(EWidgetSpace::Screen);
			ReticleComponent->SetDrawSize(ReticleDrawSize);
		}
		// Reuse old reticle widget if possible.
		UMeshComponent* MeshComponent = TargetActor->FindComponentByClass<UMeshComponent>();
		USceneComponent* ParentComponent = (MeshComponent && ReticleWidgetSocket != NAME_None) ? MeshComponent : TargetActor->GetRootComponent();
		FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, true);
		ReticleComponent->AttachToComponent(ParentComponent, AttachmentRule, ReticleWidgetSocket);
		ReticleComponent->SetVisibility(true);
		ReticleComponent->RegisterComponent();
	}
}

void URsLockOnComponent::LockOff()
{
	LockedOnTarget.Reset();

	if (ReticleComponent)
	{
		ReticleComponent->DestroyComponent();
		ReticleComponent = nullptr;
	}
}

bool URsLockOnComponent::HasLockOnTarget() const
{
	return LockedOnTarget.IsValid();
}

AActor* URsLockOnComponent::GetLockedOnTarget() const
{
	return LockedOnTarget.Get();
}
