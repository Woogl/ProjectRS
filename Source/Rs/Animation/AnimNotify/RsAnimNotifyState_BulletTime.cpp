// Copyright 2025 Team BH.


#include "RsAnimNotifyState_BulletTime.h"
#include "Rs/World/TimeControl/RsTimeControlLibrary.h"

void URsAnimNotifyState_BulletTime::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	URsTimeControlLibrary::RequestTimeDilation(MeshComp, GetFNameSafe(this), ERsTimeControlPriority::VFX, TimeDilation, TotalDuration, BlendTime);
}
