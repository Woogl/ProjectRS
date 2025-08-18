// Copyright 2025 Team BH.


#include "RsCameraAnimationActor.h"

#include "CameraAnimationSequence.h"
#include "MovieScene.h"
#include "Camera/CameraComponent.h"

ARsCameraAnimationActor::ARsCameraAnimationActor()
{
	GetCameraComponent()->SetConstraintAspectRatio(false);
}

void ARsCameraAnimationActor::BeginPlay()
{
	Super::BeginPlay();

	if (!PlayerController || !Sequence || !PlayerController->GetPawn())
	{
		return;
	}

	FTransform SpawnTransform = PlayerController->GetPawn()->GetActorTransform();
	FVector SpawnLocation = SpawnTransform.GetLocation() - FVector(0.f, 0.f, PlayerController->GetPawn()->GetDefaultHalfHeight());
	SpawnTransform.SetLocation(SpawnLocation);

	PlayerController->SetViewTarget(this);
	
	if (UCameraAnimationCameraModifier* Modifier = UCameraAnimationCameraModifier::GetCameraAnimationCameraModifierFromPlayerController(PlayerController))
	{
		CameraAnimationHandle = Modifier->PlayCameraAnimation(Sequence, Params);
	}
	
	if (!Params.bLoop)
	{
		FFrameNumber PlaybackFrames = Sequence->GetMovieScene()->GetPlaybackRange().Size<FFrameNumber>();
		float PlayLength = Sequence->GetMovieScene()->GetTickResolution().AsSeconds(PlaybackFrames);
		PlayLength = PlayLength / Params.PlayRate;
		SetLifeSpan(PlayLength);
	}
}

void ARsCameraAnimationActor::Destroyed()
{
	if (!PlayerController)
	{
		return;
	}
	
	if (UCameraAnimationCameraModifier* Modifier = UCameraAnimationCameraModifier::GetCameraAnimationCameraModifierFromPlayerController(PlayerController))
	{
		Modifier->StopCameraAnimation(CameraAnimationHandle, true);
	}
	
	if (OriginalViewTarget)
	{
		PlayerController->SetViewTarget(OriginalViewTarget);
	}
	
	Super::Destroyed();
}
