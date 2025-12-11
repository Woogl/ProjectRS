// Copyright 2025 Team BH.


#include "RsProjectilePredictionComponent.h"

#include "GameFramework/PlayerState.h"


URsProjectilePredictionComponent::URsProjectilePredictionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

float URsProjectilePredictionComponent::GetForwardPredictionTime() const
{
	const APlayerState* PS = GetPlayerState();
	return (PS && (GetNetMode() != NM_Standalone)) ? (0.001f * ClientBiasPct * FMath::Clamp(PS->ExactPing - (PS->GetOwningController()->IsLocalController() ? 0.f : PredictionLatencyReduction), 0.f, MaxPredictionPing)) : 0.f;
}

float URsProjectilePredictionComponent::GetProjectileSleepTime() const
{
	const APlayerState* PS = GetPlayerState();
	return 0.001f * FMath::Max(0.0f, PS->ExactPing - PredictionLatencyReduction - MaxPredictionPing);
}

uint32 URsProjectilePredictionComponent::GenerateNewFakeProjectileId()
{
	const uint32 NextId = FakeProjectileIdCounter;
	FakeProjectileIdCounter = FakeProjectileIdCounter < UINT32_MAX ? FakeProjectileIdCounter + 1 : 1;
	checkf(!FakeProjectiles.Contains(NextId), TEXT("Generated invalid projectile ID! ID [%i] already used. Fake projectile map size: [%i]."), NextId, FakeProjectiles.Num());
	return NextId;
}

APlayerState* URsProjectilePredictionComponent::GetPlayerState() const
{
	if (MyPlayerState)
	{
		return MyPlayerState;
	}
	MyPlayerState = Cast<APlayerController>(GetOwner())->PlayerState;
	return MyPlayerState;
}
