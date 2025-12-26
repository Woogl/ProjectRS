// Copyright 2024 Team BH.


#include "RsLockOnComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Rs/Character/Component/RsHealthComponent.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

URsLockOnComponent::URsLockOnComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	TargetingParams.Shape.ShapeType = ERsTargetingShapeType::Sphere;
	TargetingParams.Shape.HalfExtent = FVector(1000.f, 1000.f, 1000.f);
	TargetingParams.Collision.CollisionObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	TargetingParams.Sorter.ByDistance = ERsSortingOrder::Ascending;
}

void URsLockOnComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerController = Cast<AController>(GetOwner());
	if (OwnerController)
	{
		OwnerCharacter = OwnerController->GetCharacter();
	}
}

void URsLockOnComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Target = GetLockOnTarget();
	if (!Target || !OwnerController || !OwnerCharacter)
	{
		return;
	}

	FVector TargetLocation = Target->GetActorLocation();
	FVector SourceLocation = OwnerCharacter->GetActorLocation();
	if (FVector::DistSquared(TargetLocation, SourceLocation) > MaxTargetDistance * MaxTargetDistance)
	{
		LockOff();
		return;
	}

	// Update control rotation
	UGameplayCameraComponent* GameplayCameraComponent = OwnerCharacter->FindComponentByClass<UGameplayCameraComponent>();
	if (OwnerController->IsLocalPlayerController() && GameplayCameraComponent)
	{
		FRotator CurrentRotation = GameplayCameraComponent->GetEvaluatedCameraRotation();
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(SourceLocation, TargetLocation);
		TargetRotation += ControlRotationOffset;
		TargetRotation.Roll = 0.f;
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, ControlRotationInterpSpeed);
		OwnerController->SetControlRotation(NewRotation);
	}
}

bool URsLockOnComponent::LockOn(AActor* Target)
{
	if (!Target)
	{
		return false;
	}
	
	LockOnTarget = Target;
	SpawnedReticleWidget = RespawnReticleWidget(Target);

	if (URsHealthComponent* HealthComponent = LockOnTarget.Get()->FindComponentByClass<URsHealthComponent>())
	{
		HealthComponent->OnDeathStarted.AddUniqueDynamic(this, &ThisClass::HandleTargetDeath);
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

	if (UBlackboardComponent* Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(GetOwner()))
	{
		Blackboard->SetValueAsObject(BlackBoardValueName, nullptr);
	}
	
	SetComponentTickEnabled(false);
}

bool URsLockOnComponent::TargetingLockOn(const FRsTargetingParams& Params)
{
	if (AController* Controller = OwnerController.Get())
	{
		if (APawn* ControlledPawn = Controller->GetPawn())
		{
			TArray<AActor*> OutActors;
			if (URsTargetingLibrary::PerformTargeting(ControlledPawn, ControlledPawn->GetTransform(), Params, OutActors))
			{
				return LockOn(OutActors[0]);
			}
		}
	}
	return false;
}

bool URsLockOnComponent::ToggleLockOn(const FRsTargetingParams& Params)
{
	if (GetLockOnTarget() == nullptr)
	{
		return TargetingLockOn(Params);
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
		TargetingLockOn(TargetingParams);
	});
}

