// Copyright 2024 Team BH.


#include "RsAIControllerBase.h"

#include "AbilitySystemGlobals.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Rs/Camera/LockOn/RsLockOnComponent.h"
#include "Rs/Character/RsCharacterBase.h"

ARsAIControllerBase::ARsAIControllerBase()
{
	LockOnComponent = CreateDefaultSubobject<URsLockOnComponent>(TEXT("LockOnComponent"));
//	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
}

UAbilitySystemComponent* ARsAIControllerBase::GetAbilitySystemComponent() const
{
	return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetPawn());;
}

ETeamAttitude::Type ARsAIControllerBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const IGenericTeamAgentInterface* TeamInterface = Cast<IGenericTeamAgentInterface>(&Other))
	{
		if (TeamInterface->GetGenericTeamId().GetId() != GetGenericTeamId().GetId())
		{
			return ETeamAttitude::Hostile;
		}
		else
		{
			return ETeamAttitude::Friendly;
		}
	}

	return ETeamAttitude::Neutral;
}

void ARsAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PlayerController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::HandlePlayerControllingPawnChanged);
	}

	if (const TObjectPtr<APawn> MyPawn = GetPawn())
	{
		const FVector StartLocation = MyPawn->GetActorLocation();
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), StartLocation);
	}
}

void ARsAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ARsCharacterBase* RsAvatarCharacter = Cast<ARsCharacterBase>(InPawn))
	{
		if (RsAvatarCharacter->BehaviorTree)
		{
			BehaviorTree = RsAvatarCharacter->BehaviorTree;
			RunBehaviorTree(BehaviorTree);
			GetBlackboardComponent()->SetValueAsObject(TEXT("SelfActor"), InPawn);
		}
	}
}

void ARsAIControllerBase::HandlePlayerControllingPawnChanged(APawn* InOldPawn, APawn* InNewPawn)
{
	if (BehaviorTree && InNewPawn && GetBlackboardComponent())
	{
		GetBlackboardComponent()->SetValueAsObject(TEXT("PlayerControllingPawn"), InNewPawn);
	}
}
