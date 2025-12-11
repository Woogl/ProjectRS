// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Rs/Battle/Actor/RsProjectile.h"
#include "RsAbilityTask_SpawnPredictedProjectile.generated.h"

class URsProjectilePredictionComponent;
class ARsProjectile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpawnPredictedProjectileDelegate, ARsProjectile*, SpawnedProjectile);

/** Spawn information for predicted projectiles that are spawned after a delay.
 * Projectile spawning is delayed when the client's ping is higher than the maximum forward prediction ping to prevent projectiles from being forward-predicted too far. */
USTRUCT()
struct FDelayedProjectileInfo
{
	GENERATED_BODY()
	
	UPROPERTY()
	TSubclassOf<ARsProjectile> ProjectileClass = nullptr;
	
	UPROPERTY()
	FVector SpawnLocation = FVector::ZeroVector;
	
	UPROPERTY()
	FRotator SpawnRotation = FRotator::ZeroRotator;
	
	UPROPERTY()
	TWeakObjectPtr<APlayerController> PlayerController = nullptr;
	
	UPROPERTY()
	uint32 ProjectileId = INVALID_PROJECTILE_ID;
};

/** Target data for spawning projectiles. Used to send spawn information from the client to the server. */
USTRUCT()
struct FGameplayAbilityTargetData_ProjectileSpawnInfo : public FGameplayAbilityTargetData
{
	GENERATED_BODY()
	
	/** Location to spawn projectile at. */
	UPROPERTY()
	FVector SpawnLocation = FVector::ZeroVector;
	
	/** Rotation with which to spawn projectile. */
	UPROPERTY()
	FRotator SpawnRotation = FRotator::ZeroRotator;
	
	/** The projectile's ID. Used to link fake and authoritative projectiles. */
	UPROPERTY()
	uint32 ProjectileId = INVALID_PROJECTILE_ID;
	
	virtual UScriptStruct* GetScriptStruct() const override;
	virtual FString ToString() const override;
	static FGameplayAbilityTargetDataHandle MakeProjectileSpawnInfoTargetData(const FVector& SpawnLocation, const FRotator& SpawnRotation, const uint32 ProjectileId);
	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess);
};

template<>
struct TStructOpsTypeTraits<FGameplayAbilityTargetData_ProjectileSpawnInfo> : public TStructOpsTypeTraitsBase2<FGameplayAbilityTargetData_ProjectileSpawnInfo>
{
	enum
	{
		WithNetSerializer = true
	};
};

/**
 * 
 */
UCLASS()
class RS_API URsAbilityTask_SpawnPredictedProjectile : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	/**
	 * Called locally when the projectile is spawned.
	 * On clients, this will return the fake projectile actor. On the server, this will return the authoritative projectile actor.
	 * Note: This delegate may have a volatile delay at higher latencies (when projectiles are delayed), so this should not be used for gameplay logic. It is intended primarily for cosmetic effects and validation.
	 */
	UPROPERTY(BlueprintAssignable)
	FSpawnPredictedProjectileDelegate Success;
	
	/** Called on the client and server if either failed to spawn their projectile actor, usually because the ability's prediction key was rejected.
	 * The ability should likely be canceled (on both machines) at this point. */
	UPROPERTY(BlueprintAssignable)
	FSpawnPredictedProjectileDelegate FailedToSpawn;
	
	/**
	 * Spawn a predicted projectile actor. Should only be used in Local Predicted abilities.
	 * To spawn non-predicted projectiles, use a normal "Spawn Actor" task.
	 *
	 * Note: that this task will always use the parameters given by the local client.
	 * For example, if you use the player's current aim location and rotation to spawn the projectile, this task will use the client's version of those variables.
	 */
	UFUNCTION(BlueprintCallable, Meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "True"), Category="Ability|Tasks")
	static URsAbilityTask_SpawnPredictedProjectile* SpawnPredictedProjectile(UGameplayAbility* OwningAbility, TSubclassOf<ARsProjectile> ProjectileClass, FVector SpawnLocation, FRotator SpawnRotation);

	/** Spawn a fake projectile on the client and an authoritative projectile on the server.
	 * The server's projectile will be forward predicted to sync with the client's, the projectiles will be linked together, and reconciliation will be performed by the projectiles themselves. */
	virtual void Activate() override;
	
protected:
	/** Handle for spawning a fake projectile after a delay, when the client's ping is higher than the forward-prediction limit. */
	FTimerHandle SpawnDelayedFakeProjHandle;
	
	/** Cached spawn info for spawning a fake projectile after a delay. */
	UPROPERTY()
	FDelayedProjectileInfo DelayedProjectileInfo;
	
	/** Spawns a fake projectile using the DelayedProjectileInfo. */
	void SpawnDelayedFakeProjectile();
	
	/** Replicates the client's spawn data to the server, so the server can spawn the authoritative projectile. */
	void SendSpawnDataToServer(const FVector& InLocation, const FRotator& InRotation, uint32 InProjectileId);
	
	/** Sends the task cancellation to the server if the client failed. */
	void CancelServerSpawn();
	
	/** Spawns the authoritative projectile on the server when the spawn data is received from the client. */
	void OnSpawnDataReplicated(const FGameplayAbilityTargetDataHandle& Data, FGameplayTag Activation);
	
	/** Cancels this task on the server if the client failed to spawn their version of the projectile. */
	void OnSpawnDataCancelled();
	
	/** Destroys the client's fake projectile if this task is rejected (e.g. the server rejects the ability activation). */
	UFUNCTION()
	void OnTaskRejected();
	
	/** The fake projectile spawned by this task on the client. Used to destroy the spawned projectile if this task is rejected. */
	TWeakObjectPtr<ARsProjectile> SpawnedFakeProj;
	
	/** Helper to make spawn parameters for a projectile. */
	FActorSpawnParameters GenerateSpawnParams() const;
	
	/** Helper to make spawn parameters for a fake projectile. */
	FActorSpawnParameters GenerateSpawnParamsForFake(const uint32 ProjectileId) const;
	
	/** Helper to make spawn parameters for an authoritative projectile. */
	FActorSpawnParameters GenerateSpawnParamsForAuth(const uint32 ProjectileId) const;
	
	APlayerController* GetPlayerController() const;
	URsProjectilePredictionComponent* GetProjectilePredictionComponent() const;
	
protected:
	/** The projectile class to spawn. */
	UPROPERTY()
	TSubclassOf<ARsProjectile> ProjectileClass;
	
	/** Location at which to spawn the projectile. Determined by client. */
	FVector SpawnLocation;
	
	/** Rotation with which to spawn the projectile. Determined by client. */
	FRotator SpawnRotation;
};
