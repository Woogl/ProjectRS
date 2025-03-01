// Copyright 2024 Team BH.


#include "RsTargetingComponent.h"

#include "Components/WidgetComponent.h"
#include "Rs/Camera/RsCameraLibrary.h"

URsTargetingComponent::URsTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void URsTargetingComponent::LockOn(AActor* InTarget)
{
	LockedOnTarget = InTarget;

	if (LockedOnTarget.IsValid() && ReticleWidget != nullptr)
	{
		if (ReticleComponent == nullptr)
		{
			// Create new reticle widget.
			ReticleComponent = NewObject<UWidgetComponent>(InTarget);
			ReticleComponent->SetWidgetClass(ReticleWidget);
			ReticleComponent->SetWidgetSpace(EWidgetSpace::Screen);
			ReticleComponent->SetDrawSize(ReticleDrawSize);
		}
		// Reuse old reticle widget if possible.
		UMeshComponent* MeshComponent = InTarget->FindComponentByClass<UMeshComponent>();
		USceneComponent* ParentComponent = (MeshComponent && ReticleWidgetSocket != NAME_None) ? MeshComponent : InTarget->GetRootComponent();
		FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, true);
		ReticleComponent->AttachToComponent(ParentComponent, AttachmentRule, ReticleWidgetSocket);
		ReticleComponent->SetVisibility(true);
		ReticleComponent->RegisterComponent();
	}
}

void URsTargetingComponent::LockOff()
{
	LockedOnTarget.Reset();

	if (ReticleComponent)
	{
		ReticleComponent->DestroyComponent();
		ReticleComponent = nullptr;
	}
}

bool URsTargetingComponent::HasLockOnTarget() const
{
	return LockedOnTarget.IsValid();
}

AActor* URsTargetingComponent::GetLockedOnTarget() const
{
	return LockedOnTarget.Get();
}

URsTargetingComponent* URsTargetingComponent::GetTargetingComponent(AActor* Actor)
{
	if (Actor)
	{
		return Actor->FindComponentByClass<URsTargetingComponent>();
	}
	return nullptr;	
}

void URsTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void URsTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (LockedOnTarget.IsValid())
	{
		if (UGameplayCameraComponent* GameplayCamera = URsCameraLibrary::GetPlayerCameraComponent(this))
		{
			
		}
	}
}

