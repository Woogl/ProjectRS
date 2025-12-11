#pragma once

#include "CoreMinimal.h"
#include "RsProjectileTypes.generated.h"

class UForceFeedbackAttenuation;
class UNiagaraComponent;
class UNiagaraSystem;

// Info for spawning decals with ProjectileFX.
USTRUCT(BlueprintType)
struct FProjectileDecalInfo
{
	GENERATED_BODY()
	
public:
	void SpawnDecal(const UObject* WorldContextObject, const FVector& Location, const FRotator& Rotation, USceneComponent* AttachComponent);
	
	/** Decal material to spawn. */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMaterialInterface> DecalMaterial;
	
	/** Decal size in world space. */
	UPROPERTY(EditDefaultsOnly, Meta = (AllowPreserveRatio = "True"))
	FVector DecalSize = FVector(128.0f, 256.0f, 256.0f);
	
	/** How long to wait before the decal starts fading out. */
	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = "0.0", Units = "Seconds"))
	float FadeOutStartDelay = 2.f;
	
	/** How long it takes the decal actor to fade out. Set to 0 to disable fade-out. */
	UPROPERTY(EditDefaultsOnly, Meta = (ClampMin = "0.0", Units = "Seconds"))
	float FadeOutDuration = 0.f;
};

/**
 * Info for spawning force feedback in the world with ProjectileFX.
 */
USTRUCT(BlueprintType)
struct FProjectileForceFeedbackInfo
{
	GENERATED_BODY()
	
	void PlayForceFeedbackEffect(const UObject* WorldContextObject, const FVector& Location, const FRotator& Rotation);
	
	/** Force feedback effect to play. */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UForceFeedbackEffect> ForceFeedbackEffect = nullptr;
	
	/** Multiplier with which to scale the force feedback effect's intensity. */
	UPROPERTY(EditDefaultsOnly)
	float IntensityMultiplier = 1.f;
	
	/** Attenuation settings for the force feedback effect. */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UForceFeedbackAttenuation> Attenuation = nullptr;
};

/**
 * Cached references to spawned projectile effects. Used to end looping effects, but holds references to all spawned particles and sounds (until they're destroyed).
 */
USTRUCT()
struct FSpawnedProjectileFX
{
	GENERATED_BODY()

	/** Stops looping effects. */
	void StopEffects();
	
	/** Spawned FX components. May contain null pointers when non-looping FX destroy themselves. */
	TArray<TObjectPtr<UNiagaraComponent>> SpawnedParticles;
	
	/** Spawned audio components. May contain null pointers when non-looping sounds destroy themselves. */
	TArray<TObjectPtr<UAudioComponent>> SpawnedSounds;
};

// Set of non-replicated FX that can be spawned as a collection.
USTRUCT(BlueprintType)
struct FProjectileFX
{
	GENERATED_BODY()
	
public:
	/**
	 * Spawns effects with the given parameters.
	 * @param AttachComponent	Component to attach the decal to. Useful for moving environment actors.
	 * @param bSkipDecal		If true, decal is not spawned. Decals are skipped when the projectile hits a target, as
	 *							opposed to an environment surface.
	 */
	void ExecuteEffects(const UObject* WorldContextObject, const FVector& Location, const FRotator& Rotation, USceneComponent* AttachComponent, bool bSkipDecal = false);
	
	/** Ends any looping effects when the projectile dies. Does NOT kill one-shot effects. */
	void StopEffects() { SpawnedFX.StopEffects(); }
	
protected:
	/** Particle effects to spawn. Looping particles will be destroyed when the projectile dies. */
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<UNiagaraSystem>> Particles;
	
	/** Sounds to play. Looping sounds will be destroyed when the projectile dies. */
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<USoundBase>> Sounds;
	
	/** Force feedback to trigger. Cannot be looping. */
	UPROPERTY(EditDefaultsOnly)
	FProjectileForceFeedbackInfo ForceFeedback;
	
	/** Decal to spawn. */
	UPROPERTY(EditDefaultsOnly)
	FProjectileDecalInfo Decal;
	
private:
	/** Cached references to spawned particles and sounds. Used to end looping effects when the projectile dies. */
	FSpawnedProjectileFX SpawnedFX;
};