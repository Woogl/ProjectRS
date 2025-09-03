// Copyright 2025 Team BH.


#include "RsCameraAnimationActor.h"

#include "CameraAnimationSequence.h"
#include "MovieScene.h"
#include "Camera/CameraComponent.h"
#include "Rs/Player/RsPlayerController.h"

ARsCameraAnimationActor::ARsCameraAnimationActor()
{
	GetCameraComponent()->SetConstraintAspectRatio(false);
}

void ARsCameraAnimationActor::PlayCameraAnimation(ARsPlayerController* InPlayerController, UCameraAnimationSequence* InSequence, FCameraAnimationParams InParams)
{
	if (!InSequence || !InPlayerController || !InPlayerController->GetPawn())
	{
		return;
	}

	PlayerController = InPlayerController;
	OriginalViewTarget = InPlayerController->GetViewTarget();
	Sequence = InSequence;
	Params = InParams;

	FTransform SpawnTransform = PlayerController->GetPawn()->GetActorTransform();
	FVector SpawnLocation = SpawnTransform.GetLocation() - FVector(0.f, 0.f, PlayerController->GetPawn()->GetDefaultHalfHeight());
	SpawnTransform.SetLocation(SpawnLocation); SetActorTransform(SpawnTransform);
	
	PlayerController->SetViewTargetWithBlend(this, Params.EaseInDuration);
	PlayerController->CurrentAnimatonCameraActor = this;

	if (UCameraAnimationCameraModifier* Modifier = UCameraAnimationCameraModifier::GetCameraAnimationCameraModifierFromPlayerController(PlayerController))
	{
		CameraAnimationHandle = CameraAnimationHandle = Modifier->PlayCameraAnimation(Sequence, Params);
	}
	
	if (!Params.bLoop)
	{
		FFrameNumber PlaybackFrames = Sequence->GetMovieScene()->GetPlaybackRange().Size<FFrameNumber>();
		float PlayLength = Sequence->GetMovieScene()->GetTickResolution().AsSeconds(PlaybackFrames);
		PlayLength = PlayLength / Params.PlayRate;
		FTimerHandle TimerHandle;
		PlayerController->GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::HandleFinishTimer, PlayLength);
	}
}

void ARsCameraAnimationActor::ResetCameraAnimation()
{
	if (!PlayerController || !OriginalViewTarget)
	{
		return;
	}
	
	if (UCameraAnimationCameraModifier* Modifier = UCameraAnimationCameraModifier::GetCameraAnimationCameraModifierFromPlayerController(PlayerController))
	{
		Modifier->StopCameraAnimation(CameraAnimationHandle, false);
		PlayerController->SetViewTarget(OriginalViewTarget);
		PlayerController->UpdateCameraManager(0);
	}
	
	SetLifeSpan(1.f);
}

UCameraAnimationSequence* ARsCameraAnimationActor::GetSequence() const
{
	return Sequence;
}

void ARsCameraAnimationActor::HandleFinishTimer()
{
	ResetCameraAnimation();
}
