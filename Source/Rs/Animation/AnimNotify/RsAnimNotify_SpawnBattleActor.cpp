// Copyright 2025 Team BH.


#include "RsAnimNotify_SpawnBattleActor.h"

#include "Rs/Battle/Actor/RsBattleActor.h"
#include "Rs/Character/Component/RsBattleActorManagerComponent.h"

void URsAnimNotify_SpawnBattleActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner || !BattleActorClass)
	{
		return;
	}

#if WITH_EDITOR
	// Show spawned actor in editor preview
	UWorld* World = MeshComp->GetWorld();
	if (World->WorldType == EWorldType::EditorPreview)
	{
		if (AActor* PrevSpawnedActor = WeakSpawnedActor.Get())
		{
			// Destroy previous spawned actor in animation preview
			PrevSpawnedActor->Destroy();
		}
		FRotator DeltaRotator = FRotator(0.f, 90.f, 0.f);
		FQuat AdjustedRotation = DeltaRotator.Quaternion() * MeshComp->GetComponentTransform().GetRotation();
		FVector AdjustedLocation = MeshComp->GetComponentTransform().GetLocation() + AdjustedRotation.RotateVector(LocalPosition);
		FTransform SpawnTransform = FTransform(AdjustedRotation, AdjustedLocation);
		if (ARsBattleActor* SpawnedActor = Owner->GetWorld()->SpawnActor<ARsBattleActor>(BattleActorClass, SpawnTransform))
		{
			// Keep reference for spawned actor
			WeakSpawnedActor = SpawnedActor;
		}
	}
#endif // WITH_EDITOR

	if (URsBattleActorManagerComponent* BattleActorManagerComponent = Owner->GetComponentByClass<URsBattleActorManagerComponent>())
	{
		BattleActorManagerComponent->SpawnBattleActor(BattleActorClass, LocalPosition);
	}
}
