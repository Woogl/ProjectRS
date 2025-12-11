#include "RsProjectileTypes.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Components/AudioComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

void FProjectileDecalInfo::SpawnDecal(const UObject* WorldContextObject, const FVector& Location, const FRotator& Rotation, USceneComponent* AttachComponent)
{
	if (!DecalMaterial)
	{
		return;
	}
	// Decals are usually reversed.
	const FRotator InverseRotation = (Rotation.Vector() * -1.0f).Rotation();
	UDecalComponent* SpawnedDecal;
	// Attach decals in case we hit a moving actor.
	if (AttachComponent)
	{
		SpawnedDecal = UGameplayStatics::SpawnDecalAttached(DecalMaterial, DecalSize, AttachComponent, NAME_None, Location, InverseRotation, EAttachLocation::KeepWorldPosition);
	}
	else
	{
		SpawnedDecal = UGameplayStatics::SpawnDecalAtLocation(WorldContextObject, DecalMaterial, DecalSize, Location, InverseRotation);
	}
	if (SpawnedDecal)
	{
		SpawnedDecal->SetFadeOut(FadeOutStartDelay, FadeOutDuration, false);
	}
}

void FProjectileForceFeedbackInfo::PlayForceFeedbackEffect(const UObject* WorldContextObject, const FVector& Location, const FRotator& Rotation)
{
	if (!ForceFeedbackEffect)
	{
		return;
	}
	UGameplayStatics::SpawnForceFeedbackAtLocation(WorldContextObject, ForceFeedbackEffect, Location, Rotation, false, IntensityMultiplier, 0.0f, Attenuation);
}

void FSpawnedProjectileFX::StopEffects()
{
	for (UNiagaraComponent* Particle : SpawnedParticles)
	{
		if (IsValid(Particle))
		{
			if (Cast<UNiagaraSystem>(Particle->GetFXSystemAsset())->IsLooping())
			{
				// Tick looping particles on last time before stopping.
				Particle->SetForceSolo(true);
				Particle->TickComponent(0.0f, LEVELTICK_All, nullptr);
				Particle->SetForceSolo(false);
				Particle->Deactivate();
				Particle->SetAutoDestroy(true);
			}
		}
	}
	for (UAudioComponent* Sound : SpawnedSounds)
	{
		if (IsValid(Sound))
		{
			if (Sound->GetSound() && !Sound->GetSound()->IsOneShot())
			{
				Sound->Stop();
			}
		}
	}
	SpawnedParticles.Empty();
	SpawnedSounds.Empty();
}

void FProjectileFX::ExecuteEffects(const UObject* WorldContextObject, const FVector& Location, const FRotator& Rotation, USceneComponent* AttachComponent, bool bSkipDecal)
{
	// Spawn and cache particles.
	for (UNiagaraSystem* SystemToSpawn : Particles)
	{
		if (SystemToSpawn != nullptr)
		{
			if (UNiagaraComponent* SpawnedSystem = UNiagaraFunctionLibrary::SpawnSystemAtLocation(WorldContextObject, SystemToSpawn, Location, Rotation, FVector(1.0f), true, true, ENCPoolMethod::AutoRelease))
			{
				SpawnedSystem->SetCastShadow(false);
				SpawnedFX.SpawnedParticles.Add(SpawnedSystem);
			}
		}
	}
	// Spawn and cache sounds.
	for (USoundBase* SoundToSpawn : Sounds)
	{
		if (SoundToSpawn != nullptr)
		{
			if (UAudioComponent* SpawnedSound = UGameplayStatics::SpawnSoundAtLocation(WorldContextObject, SoundToSpawn,
				Location, Rotation))
			{
				SpawnedFX.SpawnedSounds.Add(SpawnedSound);
			}
		}
	}
	ForceFeedback.PlayForceFeedbackEffect(WorldContextObject, Location, Rotation);
	if (!bSkipDecal)
	{
		Decal.SpawnDecal(WorldContextObject, Location, Rotation, AttachComponent);
	}
}
