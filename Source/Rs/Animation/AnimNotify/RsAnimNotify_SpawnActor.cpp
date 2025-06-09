// Copyright 2025 Team BH.


#include "RsAnimNotify_SpawnActor.h"

#include "GenericTeamAgentInterface.h"

void URsAnimNotify_SpawnActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner || !ActorClass)
	{
		return;
	}
	
	if (AActor* SpawnedActor = Owner->GetWorld()->SpawnActorDeferred<AActor>(ActorClass, FTransform(), Owner, Cast<APawn>(Owner)))
	{
		FRotator SpawnRotation = FRotator(0.f, Owner->GetActorRotation().Yaw, 0.f);
		FVector SpawnLocation = Owner->GetActorLocation() + Owner->GetTransform().TransformVector(RelativeLocation);

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
		
		SpawnedActor->FinishSpawning(FTransform(SpawnRotation, SpawnLocation));
	}
}
