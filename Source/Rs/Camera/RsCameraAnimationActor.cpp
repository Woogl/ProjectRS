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

	PlayerController->SetViewTargetWithBlend(this, Params.EaseInDuration);
	
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
	ResetCameraAnimation();
	
	Super::Destroyed();
}

void ARsCameraAnimationActor::ResetCameraAnimation()
{
	if (!IsValid(PlayerController))
	{
		Super::Destroyed();
		return;
	}
	
	if (UCameraAnimationCameraModifier* Modifier = UCameraAnimationCameraModifier::GetCameraAnimationCameraModifierFromPlayerController(PlayerController))
	{
		bool bStopImmediately = Params.EaseOutDuration == 0.f;
		Modifier->StopCameraAnimation(CameraAnimationHandle, bStopImmediately);
	}
	
	if (IsValid(OriginalViewTarget))
	{
		PlayerController->SetViewTargetWithBlend(OriginalViewTarget, Params.EaseOutDuration);
		PlayerController->SetViewTarget(OriginalViewTarget);
	}
}
