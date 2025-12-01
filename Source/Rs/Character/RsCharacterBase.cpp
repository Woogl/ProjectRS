// Copyright 2024 Team BH.


#include "RsCharacterBase.h"

#include "Component/RsBattleActorManagerComponent.h"
#include "Component/RsCharacterMovementComponent.h"
#include "Component/RsHealthComponent.h"
#include "Component/RsStaggerComponent.h"
#include "Net/UnrealNetwork.h"
#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"

#include "Rs/Battle/RsBattleLibrary.h"
#include "Rs/UI/Component/RsNameplateComponent.h"

ARsCharacterBase::ARsCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<URsCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	HealthComponent = CreateDefaultSubobject<URsHealthComponent>(TEXT("HealthComponent"));
	StaggerComponent = CreateDefaultSubobject<URsStaggerComponent>(TEXT("StaggerComponent"));
	
	NameplateComponent = CreateDefaultSubobject<URsNameplateComponent>(TEXT("NameplateComponent"));
	NameplateComponent->SetupAttachment(RootComponent);
	NameplateComponent->SetRelativeLocation(FVector(0.f, 0.f, 120.f));

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	//GetMesh()->SetVisibility(false);
	
	CharacterAppearance = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterAppearance"));
	CharacterAppearance->SetupAttachment(GetMesh());
	CharacterAppearance->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	CharacterAppearance->SetCollisionProfileName(TEXT("CharacterMesh"));

	BattleActorManagerComponent = CreateDefaultSubobject<URsBattleActorManagerComponent>(TEXT("BattleActorManagerComponent"));
}

void ARsCharacterBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, TeamId);
}

UAbilitySystemComponent* ARsCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

URsAbilitySystemComponent* ARsCharacterBase::GetRsAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ARsCharacterBase::GetOwnedGameplayTags(FGameplayTagContainer& OutTagContainer) const
{
	if (AbilitySystemComponent)
	{
		FGameplayTagContainer OwnedTags;
		AbilitySystemComponent->GetOwnedGameplayTags(OwnedTags);
		OutTagContainer = MoveTemp(OwnedTags);
	}
}

void ARsCharacterBase::SetGenericTeamId(const FGenericTeamId& InTeamID)
{
	uint8 Id = InTeamID;
	TeamId = static_cast<ERsTeamId>(Id);
}

FGenericTeamId ARsCharacterBase::GetGenericTeamId() const
{
	return static_cast<uint8>(TeamId);
}

bool ARsCharacterBase::IsLockableTarget_Implementation() const
{
	if (URsBattleLibrary::IsDead(this))
	{
		return false;
	}
	return true;
}
