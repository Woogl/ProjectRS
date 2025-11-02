// Copyright 2024 Team BH.


#include "RsAnimNotify_HitScan.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

URsAnimNotify_HitScan::URsAnimNotify_HitScan()
{
	bIsNativeBranchingPoint = true;
	
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = true;
#endif // WITH_EDITORONLY_DATA
}

FString URsAnimNotify_HitScan::GetNotifyName_Implementation() const
{
	if (EventTag.IsValid())
	{
		FString EventTagString = EventTag.ToString();
		return EventTagString.Replace(TEXT("AnimNotify."), TEXT(""));
	}
	return Super::GetNotifyName_Implementation();
}

void URsAnimNotify_HitScan::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	UWorld* World = Owner->GetWorld();
	for (AActor* ResultActor : Targets)
	{
		FGameplayEventData Payload;
		Payload.EventTag = EventTag;
		Payload.Instigator = Owner;
		Payload.Target = ResultActor;
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Owner))
		{
			FVector Start = MeshComp->GetSocketLocation(SocketName);
			FVector End = ResultActor->GetActorLocation();
			FHitResult HitResult;
			FCollisionQueryParams Query;
			Query.AddIgnoredActor(Owner);
			World->LineTraceSingleByChannel(HitResult, Start, End, ECC_RsAttack, Query);
			URsTargetingLibrary::DrawDebugArrow(World, Start, End, FColor::Cyan);
			FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
			EffectContext.AddHitResult(HitResult);
			Payload.ContextHandle = EffectContext;
		}

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EventTag, Payload);
	}
}
