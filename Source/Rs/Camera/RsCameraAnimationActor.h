// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Animations/CameraAnimationCameraModifier.h"
#include "Camera/CameraActor.h"
#include "RsCameraAnimationActor.generated.h"

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
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UFUNCTION(BlueprintCallable)
	void ResetCameraAnimation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	TObjectPtr<UCameraAnimationSequence> Sequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	FCameraAnimationParams Params;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	TObjectPtr<AActor> OriginalViewTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	TObjectPtr<APlayerController> PlayerController;

private:
	FCameraAnimationHandle CameraAnimationHandle;
};
