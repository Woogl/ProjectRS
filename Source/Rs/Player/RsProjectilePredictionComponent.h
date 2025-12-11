// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RsProjectilePredictionComponent.generated.h"

class ARsProjectile;

UCLASS()
class RS_API URsProjectilePredictionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URsProjectilePredictionComponent();

public: // Config
	/** Aka "fudge factor."
	 * The server's perceived ping of a connected client is usually 10-20ms higher than their actual ping (due to processing time, tick rate, etc.).
	 * This is used to "fudge" the server's estimate of a client's ping to get a more accurate guess of their exact ping. */
	UPROPERTY(BlueprintReadOnly, Config, Category = Network)
	float PredictionLatencyReduction = 20.f;
	
	/** How much (from 0-1) to favor the client when determining the real position of predicted projectiles.
	 * A greater value will spawn authoritative projectiles closer to where the client wants (forwarding them more towards the fake predicted projectile);
	 * a smaller value will spawn authoritative projectiles closer to where the server wants (forwarding them less). */
	UPROPERTY(BlueprintReadOnly, Config, Category = Network)
	float ClientBiasPct = 0.5f;
	
	/** Max amount of ping to predict ahead for.
	 * If the client's ping exceeds this, we'll delay spawning the projectile so it doesn't spawn further ahead than this. */
	UPROPERTY(BlueprintReadOnly, Config, Category = Network)
	float MaxPredictionPing = 120.f;

public: // Prediction
	/** The amount of time, in seconds, to tick or simulate to make up for network lag. (1/2 player's ping) - prediction latency reduction. */
	float GetForwardPredictionTime() const;
	
	/** How long to wait before spawning the projectile if the client's ping exceeds MaxPredictionPing, so we don't forward-predict too far. */
	float GetProjectileSleepTime() const;
	
	/** List of this client's fake projectiles (client-side predicted projectiles) that haven't been linked to an authoritative projectile yet. */
	UPROPERTY()
	TMap<uint32, TObjectPtr<ARsProjectile>> FakeProjectiles;
	
	/** Generates a unique ID for a new fake projectile. */
	uint32 GenerateNewFakeProjectileId();
	
private:
	/** Internal counter for projectile IDs. Starts at 1 because 0 is reversed for non-predicted projectiles. */
	uint32 FakeProjectileIdCounter = 1;
	
	APlayerState* GetPlayerState() const;
	
	mutable TObjectPtr<APlayerState> MyPlayerState;
};
