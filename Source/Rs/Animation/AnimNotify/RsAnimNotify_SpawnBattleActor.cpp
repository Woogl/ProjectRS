// Copyright 2025 Team BH.


#include "RsAnimNotify_SpawnBattleActor.h"

#include "GenericTeamAgentInterface.h"
#include "Rs/Battle/Actor/RsBattleActor.h"

void URsAnimNotify_SpawnBattleActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner || !BattleActorClass)
	{
		return;
	}

#if WITH_EDITOR
	// Destroy previous spawned actor in animation preview
	UWorld* World = MeshComp->GetWorld();
	if (World->WorldType == EWorldType::EditorPreview)
	{
		if (AActor* PrevSpawnedActor = WeakSpawnedActor.Get())
		{
			PrevSpawnedActor->Destroy();
		}
	}
#endif // WITH_EDITOR
	
	if (ARsBattleActor* SpawnedActor = Owner->GetWorld()->SpawnActorDeferred<ARsBattleActor>(BattleActorClass, FTransform(), Owner, Cast<APawn>(Owner)))
	{
		FTransform SpawnTransform = MeshComp->GetComponentTransform();
		SpawnTransform.AddToTranslation(SpawnTransform.GetRotation().RotateVector(PositionOffset));

		// Set Team
		FGenericTeamId TeamId = FGenericTeamId::NoTeam;
		if (IGenericTeamAgentInterface* OwnerTeam = Cast<IGenericTeamAgentInterface>(Owner))
		{
			TeamId = OwnerTeam->GetGenericTeamId();
		}
		if (IGenericTeamAgentInterface* SpawnedTeam = Cast<IGenericTeamAgentInterface>(SpawnedActor))
		{
			SpawnedTeam->SetGenericTeamId(TeamId);
		}
		
		SpawnedActor->FinishSpawning(SpawnTransform);

#if WITH_EDITOR
		WeakSpawnedActor = SpawnedActor;
#endif // WITH_EDITOR
	}
}
