// Copyright 2025 Team BH.


#include "RsBattleActorManagerComponent.h"

#include "GenericTeamAgentInterface.h"
#include "Rs/Battle/Actor/RsBattleActor.h"

URsBattleActorManagerComponent::URsBattleActorManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

ARsBattleActor* URsBattleActorManagerComponent::SpawnBattleActor(const TSubclassOf<ARsBattleActor>& BattleActorClass, FVector LocalPosition)
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return nullptr;
	}
	
	if (ARsBattleActor* SpawnedActor = Owner->GetWorld()->SpawnActorDeferred<ARsBattleActor>(BattleActorClass, FTransform::Identity, Owner, Cast<APawn>(Owner)))
	{
		FTransform SpawnTransform = Owner->GetTransform();
		SpawnTransform.SetLocation(SpawnTransform.GetLocation() + SpawnTransform.GetRotation().RotateVector(LocalPosition));

		// Set Team
		FGenericTeamId OwnerTeamId = FGenericTeamId::NoTeam;
		if (IGenericTeamAgentInterface* OwnerTeam = Cast<IGenericTeamAgentInterface>(Owner))
		{
			OwnerTeamId = OwnerTeam->GetGenericTeamId();
		}
		if (IGenericTeamAgentInterface* SpawnedActorTeam = Cast<IGenericTeamAgentInterface>(SpawnedActor))
		{
			SpawnedActorTeam->SetGenericTeamId(OwnerTeamId);
		}
		
		SpawnedActor->FinishSpawning(SpawnTransform);
		ManagedBattleActors.Add(SpawnedActor);
		return SpawnedActor;
	}

	return nullptr;
}

void URsBattleActorManagerComponent::DestroyBattleActors(TSubclassOf<ARsBattleActor> BattleActorClass)
{
	for (int32 i = ManagedBattleActors.Num() - 1; i >= 0; --i)
	{
		if (ARsBattleActor* BattleActor = ManagedBattleActors[i].Get())
		{
			if (BattleActor->IsA(BattleActorClass))
			{
				BattleActor->Destroy();
				ManagedBattleActors.RemoveAt(i);
			}
		}
		else
		{
			ManagedBattleActors.RemoveAt(i);
		}
	}
}
