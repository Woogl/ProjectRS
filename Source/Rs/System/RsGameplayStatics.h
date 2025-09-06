// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "RsGameplayStatics.generated.h"

class ARsPlayerState;
class ARsPlayerController;
class ARsPlayerCharacter;
/**
 * 
 */
UCLASS()
class RS_API URsGameplayStatics : public UGameplayStatics
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "RS", meta = (WorldContext = "WorldContextObject", UnsafeDuringActorConstruction="true"))
	static ARsPlayerState* GetRsPlayerState(const UObject* WorldContextObject, int32 PlayerIndex);

	UFUNCTION(BlueprintPure, Category="RS", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static ARsPlayerController* GetRsPlayerController(const UObject* WorldContextObject, int32 PlayerIndex);

	UFUNCTION(BlueprintPure, Category="RS", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static ARsPlayerCharacter* GetRsPlayerCharacter(const UObject* WorldContextObject, int32 PlayerIndex);
};
