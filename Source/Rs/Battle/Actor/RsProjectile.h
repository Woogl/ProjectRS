// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "RsProjectileTypes.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsProjectile.generated.h"

#define INVALID_PROJECTILE_ID 0

class UGameplayEffect;
class UNiagaraSystem;
class URsGameplayAbility;
class UProjectileMovementComponent;
class UCapsuleComponent;

/**
 * Replicated movement data for projectiles' root components. More efficient than FRepMovement.
 */
USTRUCT()
struct FRepProjectileMovement
{
	GENERATED_BODY()
	
	UPROPERTY()
	FVector_NetQuantize LinearVelocity = FVector_NetQuantize::ZeroVector;
	
	UPROPERTY()
	FVector_NetQuantize Location = FVector_NetQuantize::ZeroVector;
	
	UPROPERTY()
	FRotator Rotation = FRotator::ZeroRotator;
	
	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess);
	bool operator==(const FRepProjectileMovement& Other) const;
	bool operator!=(const FRepProjectileMovement& Other) const;
};

/**
* Information about how the authoritative projectile detonated, replicated to clients to use once they catch up to the
* server.
*/
USTRUCT()
struct FDetonationInfo
{
	GENERATED_BODY()
	
	UPROPERTY()
	bool bDetonated = false;
	
	UPROPERTY()
	bool bHasDirectImpactTarget = false;
	
	UPROPERTY()
	TObjectPtr<AActor> OtherActor = nullptr;
	
	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> OtherComp = nullptr;
	
	UPROPERTY()
	FVector HitLocation = FVector::ZeroVector;
	
	UPROPERTY()
	FVector HitNormal = FVector::ZeroVector;
	
	FDetonationInfo(){};
	FDetonationInfo(bool bInDetonated, bool bInHasDirectImpactTarget, AActor* InOtherActor, UPrimitiveComponent* InOtherComp, const FVector& InHitLocation, const FVector& InHitNormal);
	
	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess);
};

template<>
struct TStructOpsTypeTraits<FDetonationInfo> : public TStructOpsTypeTraitsBase2<FDetonationInfo>
{
	enum
	{
		WithNetSerializer = true
	};
};

// How to determine the direction in which to apply effects from projectiles. 
UENUM()
enum class EEffectDirection : uint8
{
	// Apply effects in the direction that the projectile is facing.
	InProjectileDirection,
	// Apply effects in the direction that the projectile is moving. Will differ from InProjectileDirection if the projectile is rotating or "Rotation Follows Velocity" is false.
	InVelocityDirection,
	// Apply effects in the direction from the projectile's position to the target.
	FromProjectilePosition,
};

/** 
 * Redicted projectiles should ONLY be spawned on the local client and server with the "Spawn Predicted Projectile" ability task in Local Predicted abilities.
 * This task is responsible for properly spawning, initializing, linking, and reconciling predicted projectiles.
 * Non-predicted projectiles can be spawned manually (e.g. with a normal "Spawn Actor from Class" node), but this should only be done on the server. 
 */
UCLASS(Abstract, Config = Game, Meta = (ChildCanTick, ToolTip = "Base class for projectiles."))
class RS_API ARsProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	ARsProjectile(const FObjectInitializer& ObjectInitializer);
	
	/** Replicates this projectile's spawn location to simulated proxies so they can resimulate it. */
	virtual void OnConstruction(const FTransform& Transform) override;
	
	/** Initialize our ASC and player controller to replicate them to clients. */
	virtual void PreInitializeComponents() override;
	
	/** Tries to link the client's version of the authoritative projectile to its corresponding fake projectile. */
	virtual void BeginPlay() override;
	
	/** Tick client's version of the authoritative projectile forward to where it is on the server, to make up for replication time. */
	void CatchupTick(float CatchupTickDelta);
	
	/** Allows LifeSpan to be simulated on the client-side.
	 * Default implementation only tracks LifeSpan on the server, but this makes simulating projectile effects easier on clients. */
	virtual void SetLifeSpan(float InLifespan) override;
	
	/** Forcefully detonates the projectile locally when it detonates on the server, if it hasn't already. */
	virtual void TornOff() override;
	
	/**
	 * Disables this projectile (collision, movement, ambient effects, etc.) while it's pending destruction.
	 * For replication purposes, there's a delay between the projectile's impact/detonation and its destruction.
	 * This function visually destroys the projectile before the actor is actually killed.
	 */
	void ShutDown();
	
	/** Blueprint hook for ShutDown. */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "ShutDown", Meta = (ToolTip = "Disables this projectile (FX, collision, physics, etc.) while its pending destruction. Should be used instead of EndPlay (since there's a delay before projectiles are actually destroyed, for replication purposes)."))
	void K2_ShutDown();
	
	/** Destroys the fake projectile whenever the authoritative projectile is destroyed. */
	virtual void Destroyed() override;
	
	/** Purge projectiles on level reload. */
	virtual void Reset() override { Destroy(); }
	
protected:
	/** True once fully spawned, to avoid destroying replicated projectiles before they've had a chance to link to the fake projectile (which prevents attempts to reconcile missed predictions). */
	UPROPERTY()
	bool bHasSpawnedFully;
	
	/** Delay the projectile tear-off to make sure it had time to replicate its initial state. */
	FTimerHandle TearOffTimer;
	
private:
	/** Safely disables and hides this projectile without destroying it. */
	void DisableAndHide();
	
public:
	/** Initialize this projectile as the fake projectile. */
	void InitFakeProjectile(APlayerController* OwningPlayer, uint32 InProjectileId);
	
	/** Initialize this projectile's ID. */
	FORCEINLINE void InitProjectileId(uint32 InProjectileId) { ProjectileId = InProjectileId; }
	
protected:
	/** Whether this projectile is a fake client-side projectile. Set before BeginPlay. */
	bool bIsFakeProjectile;
	
	/** This projectile's ID. Used to link fake and authoritative projectiles.
	 * Only valid on the owning client (i.e. the client with the fake projectile).
	 * NULL_PROJECTILE_ID on other machines. Set before BeginPlay. */
	UPROPERTY(Replicated)
	uint32 ProjectileId;
	/** The fake projectile that is representing this actor on the owning client, if this actor is the authoritative
	 * server-side projectile. */
	UPROPERTY()
	TObjectPtr<ARsProjectile> LinkedFakeProjectile;
	
	/** The authoritative server-side projectile that is representing this actor on the server and remote clients, if this actor is the fake client-side projectile. */
	UPROPERTY()
	TObjectPtr<ARsProjectile> LinkedAuthProjectile;
	
	/** If true, the fake projectile will lerp towards the position of the real projectile over time until they're synchronized.
	 * Only recommended for slow-moving projectiles. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	bool bCorrectFakeProjectilePositionOverTime;
	
	/** Link this authoritative projectile with its corresponding fake projectile. */
	void LinkFakeProjectile(ARsProjectile* InFakeProjectile);
	
	/** When called on an authoritative projectile, switches to using that authoritative projectile and destroys the fake projectile.
	 * Useful for missed predictions, when a fake projectile mistakenly detonates, and we want to
	 * reconcile by switching to using the real one. */
	void SwitchToRealProjectile();
	
	/** Lerps the fake projectile towards the real one each tick if bCorrectFakeProjectilePositionOverTime is true. */
	virtual void Tick(float DeltaSeconds) override;
	
	/** Called each tick to lerp the fake projectile towards the authoritative one. */
	void CorrectionLerpTick(float DeltaTime);
	
private:
	/** The initial distance from the fake projectile to its corresponding authoritative projectile.
	 * Used to correct the fake projectile's position over time. */
	float InitialProjectileError;
	
	/** After the fake projectile detonates, if the real projectile doesn't detonate in time, we switch to it and consider the fake projectile's detonation a missed prediction. */
	FTimerHandle SwitchToAuthTimer;
	
protected:
	/** Root collision component. This determines collision and the projectile's movement. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<UCapsuleComponent> CollisionComp;

public:
	/** Projectile movement component. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	
	/** Whether this projectile has detonated or not. Prevents multiple applications of effects and helps detect missed predictions. */
	bool bDetonated;
	
	/** Re-entrance guard for OnHitboxOverlapBegin. */
	bool bInOverlap;
	
	/** Called when this projectile bounces off a surface. Triggers the Bounce FX. */
	UFUNCTION()
	virtual void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);
	
	/** Called when this projectile's hitbox component overlaps an actor. Triggers detonation if the overlapping actor is a valid target. */
	UFUNCTION()
	virtual void OnHitboxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	/** Called when this projectile stops moving (usually because the collision component hit a blocking surface). Triggers detonation. */
	UFUNCTION()
	virtual void OnStop(const FHitResult& Hit);
	
public:
	/** Gather replicated movement if bReplicateProjectileMovement is set. */
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
	
	/** Fills ReplicatedProjectileMovement. */
	virtual void GatherCurrentMovement() override;
	
	/** Apply replicated velocity to this projectile's movement component. */
	virtual void PostNetReceiveVelocity(const FVector& NewVelocity) override;
	
	/** Use this projectile's root component's velocity as the actor velocity. */
	virtual FVector GetVelocity() const override;
	
	/** Remove the IsReplicatingMovement() condition. We use bReplicateProjectileMovement instead. */
	virtual void OnRep_ReplicatedMovement() override;
	
	/** Remove the IsReplicatingMovement() condition from AttachmentReplication. */
	virtual void GetReplicatedCustomConditionState(FCustomPropertyConditionState& OutActiveState) const override;
	
protected:
	/** Enables movement replication. Use this instead of bReplicateMovement.
	 * This should usually not be enabled by default, because we want projectiles simulate locally.
	 * Its primary use is replicating the final position of the authoritative projectile to clients, to ensure they detonate in the correct location. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replication")
	bool bReplicateProjectileMovement;
	
	/** Used to replicate projectile movement. */
	UPROPERTY(ReplicatedUsing = OnRep_RepProjectileMovement)
	FRepProjectileMovement ReplicatedProjectileMovement;
	
	/** Applies replicated projectile movement on clients. */
	UFUNCTION()
	void OnRep_RepProjectileMovement();
	
	/** Timer to defer this projectile's detonation on non-owning simulated proxies. We wait until the projectile has
	 * finished resimulating to trigger its detonation. */
	FTimerHandle FinishedResimulationTimer;
	
	/** Whether this projectile, if a non-owning simulated proxy, has finished locally resimulating the real  projectile, and can safely be detonated (without the risk of detonating when the real projectile didn't). */
	bool bFinishedResim;

protected:
	/** The gameplay effect to apply when this projectile hits a direct target (as opposed to an AOE target). Put a cue trigger in this effect if you want impact FX. */
	// TODO: Make bp-implementable GetImpactGameplayEffect to let projectiles choose a dynamic effect at runtime.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<UGameplayEffect> ImpactGameplayEffect;
	
	/** Which direction to apply the impact gameplay effect in. Used for gameplay effects (e.g. direction of knockback), not VFX. (Area effects are always applied with FromProjectilePosition.) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile", Meta = (EditCondition = "ImpactGameplayEffect != nullptr"))
	EEffectDirection ImpactEffectDirection;
	
	/** Called when this projectile stops because it hit a valid target, hit a blocking surface, or stopped moving (e.g. ran out of bounces).
	 * Applies ImpactGameplayEffect to the hit actor (if bHasDirectImpactTarget is true), triggers VFX and optional AOE effects, and shuts down this projectile.
	 * This is the final projectile event replicated to clients, and is synced across all machines. */
	UFUNCTION()
	void Detonate(bool bHasDirectImpactTarget, AActor* OtherActor, UPrimitiveComponent* OtherComp, const FVector& HitLocation, const FVector& HitNormal);
	
	/**
	 * Blueprint hook for post-detonation effects (e.g. impaling a spear projectile into the hit target).
	 * @param bHasDirectImpactTarget		Whether this projectile detonated against a valid target using its hitbox
	 *										component, as opposed to hitting the environment.
	 * @param HitActor						The actor that the projectile detonated against. If HasDirectImpactTarget
	 *										and UseFilter are both true, then this actor is guaranteed to have an ASC.
	 */
	UFUNCTION(BlueprintImplementableEvent, Meta = (ToolTip = "Called when the projectile detonates. \n\nThis is only called when this projectile's FX are triggered, so it will always fire exactly once per projectile on each client, and can be safely used to execute additional FX or gameplay. If \"Predict FX\" is enabled, this will be fired predictively, and may be fired twice due to missed predictions (the second time being on the authoritative projectile)."))
	void OnDetonate(bool bHasDirectImpactTarget, AActor* HitActor, UPrimitiveComponent* HitComp, FVector HitLocation, FVector HitNormal);
	
	/** If this is the owning client's version of the authoritative projectile, whether it should trigger FX (i.e. visuals, audio, etc.).
	 * This function is primarily checking for missed predictions. */
	bool ShouldAuthProjDetonateOnOwner(bool bLog = false) const;

protected:
	/** This projectile's area-of-effect radius.
	 * If greater than 0, the AOE Gameplay Effect will be applied to all actors within this radius on detonation. Set to 0 to disable. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Area of Effect", DisplayName = "Area of Effect Radius", Meta = (ClampMin = "0.0", UIMin = "0.0"))
	float AreaRadius;
	
	/** The relative offset (from the center of the collision component) where the center of the AOE radius will be.
	 * Z offset is applied in world space. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Area of Effect", DisplayName = "Area of Effect Offset", Meta = (EditCondition = "AreaRadius > 0.0"))
	FVector AreaOffset;
	
	/** GE to apply to actors within AoE Radius.  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Area of Effect", DisplayName = "Area of Effect Gameplay Effect", Meta = (EditCondition = "AreaRadius > 0.0"))
	TSubclassOf<UGameplayEffect> AreaGameplayEffect;
	
	/** If true, this projectile will NOT apply the AOE gameplay effect to targets it hits directly. Useful if you want to apply different effects to targets this projectile hits directly, and targets within its explosion radius. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Area of Effect", DisplayName = "Skip AOE Effect for Impact Target?", Meta = (EditCondition = "AreaRadius > 0.0"))
	bool bSkipAreaEffectForImpactTarget;
	
	/** Helper to calculate the origin of this projectile's AOE radius, if it has one. */
	FORCEINLINE FVector GetAreaOfEffectOrigin() const;
	
private:
	/** Helper for applying a gameplay effect (ImpactGameplayEffect or AreaGameplayEffect) to a target actor. */
	void ApplyEffectToTarget(TSubclassOf<UGameplayEffect> GE, const AActor* Target, const FHitResult& Hit) const;

protected:
	/** Whether this projectile's impact and detonation FX should be predicted. We generally don't want to predict
	 * big or slow projectiles with large AOE effects (like a rocket's explosion VFX), but we can predict the impact of
	 * smaller, fast projectiles that don't have AOE effects (like a shuriken's missed impact VFX). */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|FX")
	bool bPredictFX;
	
	/** Spawned when this projectile detonates without a target (i.e. when it hits the environment). To spawn FX when
	 * this projectile detonates WITH a target, put a cue in the Impact Gameplay Effect. */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|FX")
	FProjectileFX MissedImpactFX;
	
	/** Spawned when this projectile detonates (regardless of whether it hit a surface or a target). */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|FX")
	FProjectileFX DetonationFX;
	
	/** Spawned when this projectile bounces. */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile|FX")
	FProjectileFX BounceFX;
	
	/**
	 * How long to keep the projectile alive after it detonates (e.g. to allow it to play a "landed" animation or to
	 * stick in a wall).
	 *
	 * This is clamped internally to make sure the projectile has enough time to finish replicating before dying. If
	 * this is set to a value lower than the minimum life span required by this projectile, the projectile will instead
	 * be hidden after this timer expires, until it can be safely destroyed.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|FX", Meta = (Units = "Seconds"))
	float LifeSpanAfterDetonation;
	
	/** Timer to visually shut down this projectile (hide its meshes, kill its VFX, etc.) before destroying it.
	 * Projectiles' destruction is delayed to ensure they have time to replicate, but they should usually be visually
	 * destroyed before then. */
	FTimerHandle ShutDownTimer;
	
	/** Whether this projectile was used for FX.
	 * If a projectile isn't used for FX, it will always be hidden (or destroyed, if safe) when shut down. Otherwise, it will be kept visible according to LifeSpanAfterDetonation. */
	bool bTriggeredFX;
	
protected:
	/** The owning player's controller, cached in case our instigator dies during the projectile's lifetime. */
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Projectile")
	TWeakObjectPtr<APlayerController> PlayerController;
	/** The transform with which this projectile was spawned. Used to resimulate the projectile on non-owning simulated
	 * proxies, to prevent projectiles from appearing far ahead of where they were spawned. */
	
	UPROPERTY(Replicated)
	FTransform SpawnTransform;
	
	/** Information about this projectile's detonation, replicated to non-owning simulated proxies so they can properly
	 * detonate once they finish resimulating (since their resimulation will likely end at a slightly different time
	 * and location than the authoritative projectile). */
	UPROPERTY(ReplicatedUsing = OnRep_DetonationInfo)
	FDetonationInfo DetonationInfo;
	
	/** 
	 * Triggers detonation on non-owning simulated proxies. Since simulated proxies rewind and resimulate, they're
	 * always behind the authoritative projectile, which makes their local hit detection unreliable. Instead of letting
	 * them trigger their detonation locally, we wait for the server to tell us that the authoritative projectile
	 * detonated.
	 *
	 * If detonation info is sent by the server before the simulated proxy has finished spawning, we start our
	 * resimulation early (instead of waiting for BeginPlay) and adjust our resimulation speed to ensure the projectile
	 * is always at least briefly visible. Otherwise, we extrapolate how much time is left in our resimulation
	 * (depending on our distance from the detonation location) and set a timer to detonate when we finish.
	 */
	UFUNCTION()
	void OnRep_DetonationInfo();
	
	/** Calls Detonate using the current values of DetonationInfo at parameters. */
	void DetonateWithDetonationInfo();
	
	/** Whether this projectile is on the server. */
	bool IsServerProjectile() const;
	
private:
	/** How long to delay tearing off this projectile on the server to ensure the initial replication is sent first. */
	UPROPERTY(Config)
	float TearOffDelay;
	
	/** The minimum amount of time that this projectile needs to be visible.
	 * If this projectile detonates on spawn, it will be rewound and resimulated at a low speed to ensure it's at least briefly visible to players. */
	UPROPERTY(Config)
	float MinLifetime;
	
#if WITH_EDITOR // Debugging.
public:
	/** Performs debug drawing for this projectile, depending on which projectile it is and current debug settings.
	 * @see UGASDeveloperSettings */
	void DrawDebug();
	
	/** Draw this projectile's detonation information, if bDrawFinalPosition is enabled. */
	void DrawDetonationInfo(const FVector& Location, const FVector& Normal) const;
	
	/** Draws a single step for a projectile. */
	void DrawProjectileStep(const FColor& Color, bool bDrawDot = false, bool bThick = false) const;
	
	/** Returns the color to use for this projectile's debug draws. */
	FColor GetDebugColor() const;
	
	/** Looping timer for drawing each step of a projectile's trajectory. */
	FTimerHandle DrawDebugTimer;
#endif // WITH_EDITOR
};
