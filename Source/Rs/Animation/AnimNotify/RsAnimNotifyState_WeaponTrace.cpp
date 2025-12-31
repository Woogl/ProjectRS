// Copyright 2025 Team BH.


#include "RsAnimNotifyState_WeaponTrace.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Rs/RsLogChannels.h"
#include "Rs/Battle/Actor/RsWeapon.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

URsAnimNotifyState_WeaponTrace::URsAnimNotifyState_WeaponTrace()
{
	WeaponID = ARsWeapon::DefaultWeaponID;
}

void URsAnimNotifyState_WeaponTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}
	
	if (!PassCondition(Owner))
	{
		return;
	}
	
	TArray<AActor*> Targets = FindTargets(MeshComp);
	SendHitEvent(MeshComp, Targets);
}

void URsAnimNotifyState_WeaponTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}
	
	if (!PassCondition(Owner))
	{
		return;
	}

	TArray<AActor*> Targets = FindTargets(MeshComp);
	SendHitEvent(MeshComp, Targets);
}

void URsAnimNotifyState_WeaponTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	TArray<AActor*> Targets = FindTargets(MeshComp);
	SendHitEvent(MeshComp, Targets);
	
	RuntimeDataMap.Remove(MeshComp);
}

ARsWeapon* URsAnimNotifyState_WeaponTrace::FindWeaponActor(USkeletalMeshComponent* OwnerMeshComp) const
{
	AActor* Owner = OwnerMeshComp->GetOwner();
	
	TArray<AActor*> OutChildActors;
	Owner->GetAttachedActors(OutChildActors);
	for (AActor* ChildActor : OutChildActors)
	{
		if (ARsWeapon* Weapon = Cast<ARsWeapon>(ChildActor))
		{
			if (Weapon->GetWeaponID() == WeaponID)
			{
				return Weapon;
			}
		}
	}
	UE_LOG(LogRs, Warning, TEXT("Cannot find %s's weapon"), *Owner->GetName());
	return nullptr;
}

FRsTargetingShape URsAnimNotifyState_WeaponTrace::MakeWeaponShape(ARsWeapon* Weapon) const
{
	if (UPrimitiveComponent* WeaponPrimitive = Weapon->FindComponentByClass<UPrimitiveComponent>())
	{
		FRsTargetingShape Shape;
		if (WeaponPrimitive->IsA(UBoxComponent::StaticClass()))
		{
			Shape.ShapeType = ERsTargetingShapeType::Box;
		}
		else if	(WeaponPrimitive->IsA(UCapsuleComponent::StaticClass()))
		{
			Shape.ShapeType = ERsTargetingShapeType::Capsule;
		}
		else if (WeaponPrimitive->IsA(USphereComponent::StaticClass()))
		{
			Shape.ShapeType = ERsTargetingShapeType::Sphere;
		}
		else
		{
			Shape.ShapeType = ERsTargetingShapeType::Box;
		}
		Shape.HalfExtent = WeaponPrimitive->GetLocalBounds().BoxExtent;
		return Shape;
	}
	UE_LOG(LogRs, Warning, TEXT("Cannot make %s's shape"), *Weapon->GetName());
	return FRsTargetingShape();
}

TArray<AActor*> URsAnimNotifyState_WeaponTrace::FindTargets(USkeletalMeshComponent* OwnerMeshComp)
{
	TArray<AActor*> OutTargets;
	if (FWeaponTraceRuntimeData* Data = RuntimeDataMap.Find(OwnerMeshComp))
	{
		FTransform CurrentTransform = Data->Weapon->FindComponentByClass<UPrimitiveComponent>()->GetComponentTransform();
		FRsTargetingShape WeaponShape = MakeWeaponShape(Data->Weapon.Get());
		FRsTargetingParams TargetingParams(WeaponShape, Filter, Sorter);
		URsTargetingLibrary::PerformTargetingSwept(OwnerMeshComp->GetOwner(), Data->LastTransform, CurrentTransform, TargetingParams, OutTargets);
		
		Data->LastTransform = CurrentTransform;
	}
	else
	{
		if (ARsWeapon* Weapon = FindWeaponActor(OwnerMeshComp))
		{
			FRsTargetingShape WeaponShape = MakeWeaponShape(Weapon);
			UPrimitiveComponent* WeaponPrimitive = Weapon->FindComponentByClass<UPrimitiveComponent>();
	
			FRsTargetingParams TargetingParams(WeaponShape, Filter, Sorter);
			URsTargetingLibrary::PerformTargetingFromComponent(WeaponPrimitive, TargetingParams, OutTargets);
		
			FWeaponTraceRuntimeData NewData;
			NewData.Weapon = Weapon;
			NewData.LastTransform = WeaponPrimitive->GetComponentTransform();
			RuntimeDataMap.Add(OwnerMeshComp, NewData);
		}
	}
	return OutTargets;
}

void URsAnimNotifyState_WeaponTrace::SendHitEvent(USkeletalMeshComponent* OwnerMeshComp, TArray<AActor*> Targets)
{
	AActor* Owner = OwnerMeshComp->GetOwner();
	FWeaponTraceRuntimeData* Data = RuntimeDataMap.Find(OwnerMeshComp);
	if (!EventTag.IsValid() || !Owner || !Data)
	{
		return;
	}
	
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
	{
		for (AActor* Target : Targets)
		{
			if (!Target || Data->HitActors.Contains(Target))
			{
				continue;
			}
			else
			{
				Data->HitActors.Add(Target);
			}
			FGameplayEventData Payload;
			Payload.EventTag = EventTag;
			Payload.Instigator = Owner;
			Payload.Target = Target;
			ASC->HandleGameplayEvent(EventTag, &Payload);
		}
	}
}
