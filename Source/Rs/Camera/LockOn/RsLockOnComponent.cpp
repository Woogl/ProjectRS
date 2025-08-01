// Copyright 2024 Team BH.


#include "RsLockOnComponent.h"

#include "RsLockOnInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/WidgetComponent.h"
#include "Rs/Character/Component/RsHealthComponent.h"
#include "Rs/Player/RsPlayerController.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

URsLockOnComponent::URsLockOnComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	TargetingShape.ShapeType = ERsTargetingShapeType::Sphere;
	TargetingShape.HalfExtent = FVector(1000.f, 1000.f, 1000.f);
	
	TargetingSorter.ByDistance = ERsSortingOrder::Ascending;
}

void URsLockOnComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Target = GetLockOnTarget();
	if (!Target)
	{
		return;
	}
	AController* Controller = Cast<AController>(GetOwner());
	if (!Controller)
	{
		return;
	}
	APawn* ControlledPawn = Controller->GetPawn();
	if (!ControlledPawn)
	{
		return;
	}

	FVector TargetLocation = Target->GetActorLocation();
	FVector SourceLocation = ControlledPawn->GetActorLocation();
	if (FVector::DistSquared(TargetLocation, SourceLocation) > MaxTargetDistance * MaxTargetDistance)
	{
		LockOff();
	}
}

void URsLockOnComponent::SetTargetingParams(FRsTargetingShape Shape, FRsTargetingCollision Collision, FRsTargetingFilter Filter, FRsTargetingSorter Sorter)
{
	TargetingShape = Shape;
	TargetingCollision = Collision;
	TargetingFilter = Filter;
	TargetingSorter = Sorter;
}

bool URsLockOnComponent::LockOn(AActor* Target)
{
	if (!Target)
	{
		return false;
	}
	
	if (IRsLockOnInterface* LockOnInterface = Cast<IRsLockOnInterface>(Target))
	{
		if (LockOnInterface->Execute_IsLockableTarget(Target) == false)
		{
			return false;
		}
	}
	
	LockOnTarget = Target;
	SpawnedReticleWidget = RespawnReticleWidget(Target);

	if (URsHealthComponent* HealthComponent = LockOnTarget.Get()->FindComponentByClass<URsHealthComponent>())
	{
		HealthComponent->OnDeathStarted.AddUniqueDynamic(this, &ThisClass::HandleTargetDeath);
	}
	
	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(GetOwner()))
	{
		RsPlayerController->CameraRig = ERsCameraRig::LockOn;
	}

	if (UBlackboardComponent* Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(GetOwner()))
	{
		Blackboard->SetValueAsObject(BlackBoardValueName, Target);
	}

	SetComponentTickEnabled(true);
	return true;
}

void URsLockOnComponent::LockOff()
{
	if (LockOnTarget.IsValid())
	{
		if (URsHealthComponent* HealthComponent = LockOnTarget.Get()->FindComponentByClass<URsHealthComponent>())
		{
			HealthComponent->OnDeathStarted.RemoveAll(this);
		}
		LockOnTarget.Reset();
	}

	if (SpawnedReticleWidget.IsValid())
	{
		SpawnedReticleWidget.Get()->DestroyComponent();
		SpawnedReticleWidget.Reset();
	}
	
	if (ARsPlayerController* RsPlayerController = Cast<ARsPlayerController>(GetOwner()))
	{
		RsPlayerController->CameraRig = ERsCameraRig::ThirdPerson;
	}

	if (UBlackboardComponent* Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(GetOwner()))
	{
		Blackboard->SetValueAsObject(BlackBoardValueName, nullptr);
	}
	
	SetComponentTickEnabled(false);
}

bool URsLockOnComponent::TryTargetingLockOn(FRsTargetingShape Shape, FRsTargetingCollision Collision, FRsTargetingFilter Filter, FRsTargetingSorter Sorter)
{
	if (AController* Controller = Cast<AController>(GetOwner()))
	{
		if (APawn* ControlledPawn = Controller->GetPawn())
		{
			TArray<AActor*> OutActors;
			if (URsTargetingLibrary::PerformTargeting(ControlledPawn, ControlledPawn->GetTransform(), Shape, Collision, Filter, Sorter, OutActors))
			{
				return LockOn(OutActors[0]);
			}
		}
	}
	return false;
}

bool URsLockOnComponent::ToggleLockOn()
{
	if (GetLockOnTarget() == nullptr)
	{
		return TryTargetingLockOn(TargetingShape, TargetingCollision, TargetingFilter, TargetingSorter);
	}
	LockOff();
	return false;
}

AActor* URsLockOnComponent::GetLockOnTarget() const
{
	return LockOnTarget.Get();
}

UWidgetComponent* URsLockOnComponent::RespawnReticleWidget(AActor* Target)
{
	if (!ReticleWidget || !Target)
	{
		return nullptr;
	}
	
	// Destroy old reticle widdget.
	if (UWidgetComponent* OldReticleWidget = SpawnedReticleWidget.Get())
	{
		OldReticleWidget->DestroyComponent();
		SpawnedReticleWidget.Reset();
	}

	// Create new reticle widget.
	if (UWidgetComponent* NewReticleWidget = NewObject<UWidgetComponent>(Target))
	{
		NewReticleWidget->SetWidgetClass(ReticleWidget);
		NewReticleWidget->SetWidgetSpace(EWidgetSpace::Screen);
		NewReticleWidget->SetDrawSize(ReticleDrawSize);
		UMeshComponent* MeshComponent = Target->FindComponentByClass<UMeshComponent>();
		USceneComponent* ParentComponent = (MeshComponent && ReticleWidgetSocket != NAME_None) ? MeshComponent : Target->GetRootComponent();
		FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, true);
		NewReticleWidget->AttachToComponent(ParentComponent, AttachmentRule, ReticleWidgetSocket);
		NewReticleWidget->SetVisibility(true);
		NewReticleWidget->RegisterComponent();
		return NewReticleWidget;
	}
	
	return nullptr;
}

void URsLockOnComponent::HandleTargetDeath(AActor* DeadActor)
{
	LockOff();
	
	// Target will dead in next tick, so try lock on next tick.
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		TryTargetingLockOn(TargetingShape, TargetingCollision, TargetingFilter, TargetingSorter);
	});
}

