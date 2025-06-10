// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RsBattleActorManagerComponent.generated.h"


class ARsBattleActor;

/**
 * Keep references about summoned actors.
 * (EX: Summoned creatures, turrets, or mines.)
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RS_API URsBattleActorManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URsBattleActorManagerComponent();
	
	ARsBattleActor* SpawnBattleActor(const TSubclassOf<ARsBattleActor>& BattleActorClass, FVector LocalPosition);
	void DestroyBattleActors(TSubclassOf<ARsBattleActor> BattleActorClass);

protected:
	TArray<TWeakObjectPtr<ARsBattleActor>> ManagedBattleActors;
};
