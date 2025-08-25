// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animations/CameraAnimationCameraModifier.h"
#include "Camera/CameraActor.h"
#include "RsCameraAnimationActor.generated.h"

class ARsPlayerController;
class UCameraAnimationSequence;
/**
 * 
 */
UCLASS()
class RS_API ARsCameraAnimationActor : public ACameraActor
{
	GENERATED_BODY()

public:
	ARsCameraAnimationActor();

	UFUNCTION(BlueprintCallable)
	void PlayCameraAnimation(ARsPlayerController* InPlayerController, UCameraAnimationSequence* InSequence, FCameraAnimationParams InParams);

	UFUNCTION(BlueprintCallable)
	void ResetCameraAnimation();

	UCameraAnimationSequence* GetSequence() const;

private:
	UPROPERTY()
	TObjectPtr<UCameraAnimationSequence> Sequence;

	UPROPERTY()
	FCameraAnimationParams Params;

	UPROPERTY()
	TObjectPtr<AActor> OriginalViewTarget;
	
	UPROPERTY()
	TObjectPtr<ARsPlayerController> PlayerController;
	
	void HandleFinishTimer();
	FCameraAnimationHandle CameraAnimationHandle;
};
