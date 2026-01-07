// Copyright 2025 Team BH.


#include "RsAnimNotifyState_WeaponTrace.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/GameplayAbilityTypes.h"
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
	
	AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr;
	if (!Owner || !PassCondition(Owner))
	{
		RuntimeDataMap.Remove(MeshComp);
		return;
	}
	
	TArray<AActor*> Targets = FindTargets(MeshComp);
	SendHitEvent(MeshComp, Targets);
}

void URsAnimNotifyState_WeaponTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr;
	if (!Owner || !PassCondition(Owner))
	{
		RuntimeDataMap.Remove(MeshComp);
		return;
	}

	TArray<AActor*> Targets = FindTargets(MeshComp);
	SendHitEvent(MeshComp, Targets);
}

void URsAnimNotifyState_WeaponTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	
	AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr;
	if (!Owner || !PassCondition(Owner))
	{
		RuntimeDataMap.Remove(MeshComp);
		return;
	}
	
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

TArray<AActor*> URsAnimNotifyState_WeaponTrace::FindTargets(USkeletalMeshComponent* OwnerMeshComp)
{
	TArray<AActor*> OutTargets;
	if (FWeaponTraceRuntimeData* Data = RuntimeDataMap.Find(OwnerMeshComp))
	{
		if (ARsWeapon* Weapon = Data->Weapon.Get())
		{
			FTransform CurrentTransform = Weapon->GetRootPrimitive()->GetComponentTransform();
			FRsTargetingShape WeaponShape = Weapon->GetWeaponShape();
			FRsTargetingParams TargetingParams(WeaponShape, Filter, Sorter);
			URsTargetingLibrary::PerformTargetingSwept(OwnerMeshComp->GetOwner(), Data->LastTransform, CurrentTransform, TargetingParams, OutTargets);
		
			Data->LastTransform = CurrentTransform;
		}
	}
	else
	{
		if (ARsWeapon* Weapon = FindWeaponActor(OwnerMeshComp))
		{
			UPrimitiveComponent* WeaponRoot = Weapon->GetRootPrimitive();
			FRsTargetingShape WeaponShape = Weapon->GetWeaponShape();
			FRsTargetingParams TargetingParams(WeaponShape, Filter, Sorter);
			URsTargetingLibrary::PerformTargetingFromComponent(WeaponRoot, TargetingParams, OutTargets);
		
			FWeaponTraceRuntimeData NewData;
			NewData.Weapon = Weapon;
			NewData.LastTransform = WeaponRoot->GetComponentTransform();
			RuntimeDataMap.Add(OwnerMeshComp, NewData);
		}
	}
	return OutTargets;
}

void URsAnimNotifyState_WeaponTrace::SendHitEvent(USkeletalMeshComponent* OwnerMeshComp, const TArray<AActor*>& Targets)
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
