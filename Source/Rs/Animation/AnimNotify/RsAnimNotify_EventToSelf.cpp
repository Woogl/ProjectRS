// Copyright 2024 Team BH.


#include "RsAnimNotify_EventToSelf.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

URsAnimNotify_EventToSelf::URsAnimNotify_EventToSelf()
{
}

#if WITH_EDITOR
bool URsAnimNotify_EventToSelf::CanEditChange(const FProperty* InProperty) const
{
	const bool ParentVal = Super::CanEditChange(InProperty);
	
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, SocketName))
	{
		return ParentVal && !bAlwaysSuccess;
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, PositionOffset))
	{
		return ParentVal && !bAlwaysSuccess;
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, RotationOffset))
	{
		return ParentVal && !bAlwaysSuccess;
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, Shape))
	{
		return ParentVal && !bAlwaysSuccess;
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, Collision))
	{
		return ParentVal && !bAlwaysSuccess;
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, Filter))
	{
		return ParentVal && !bAlwaysSuccess;
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, Sorter))
	{
		return ParentVal && !bAlwaysSuccess;
	}

	return ParentVal;
}
#endif // WITH_EDITOR

void URsAnimNotify_EventToSelf::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!bAlwaysSuccess)
	{
		if (!PerformTargeting(MeshComp))
		{
			return;
		}
	}

	if (!EventTag.IsValid())
	{
		return;
	}
	
	AActor* Owner = MeshComp->GetOwner();
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
	{
		FGameplayEventData Payload;
		Payload.EventTag = EventTag;
		Payload.Instigator = Owner;
		Payload.Target = Owner;
		ASC->HandleGameplayEvent(EventTag, &Payload);
	}
}
