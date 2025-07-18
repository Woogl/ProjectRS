// Copyright 2025 Team BH.


#include "RsAnimNotify_DespawnBattleActors.h"

#include "Kismet/GameplayStatics.h"
#include "Rs/Battle/Actor/RsBattleActor.h"
#include "Rs/Character/Component/RsBattleActorManagerComponent.h"

URsAnimNotify_DespawnBattleActors::URsAnimNotify_DespawnBattleActors()
{
	bIsNativeBranchingPoint = true;
	
#if WITH_EDITORONLY_DATA
	bShouldFireInEditor = true;
#endif // WITH_EDITORONLY_DATA
}

void URsAnimNotify_DespawnBattleActors::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner || !BattleActorClass)
	{
		return;
	}

#if WITH_EDITOR
	// Despawn all BattleActors in editor preview
	UWorld* World = MeshComp->GetWorld();
	if (World->WorldType == EWorldType::EditorPreview)
	{
		TArray<AActor*> SpawnedActors;
		UGameplayStatics::GetAllActorsOfClass(World, BattleActorClass, SpawnedActors);
		for (AActor* SpawnActor : SpawnedActors)
		{
			SpawnActor->Destroy();
		}
	}
#endif // WITH_EDITOR
	
	if (URsBattleActorManagerComponent* BattleActorManagerComponent = Owner->GetComponentByClass<URsBattleActorManagerComponent>())
	{
		BattleActorManagerComponent->DespawnBattleActors(BattleActorClass);
	}
}
