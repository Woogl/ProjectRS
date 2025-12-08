// Copyright 2024 Team BH.


#include "RsCharacterBase.h"

#include "Component/RsBattleActorManagerComponent.h"
#include "Component/RsCharacterMovementComponent.h"
#include "Component/RsHealthComponent.h"
#include "Component/RsRagdollComponent.h"
#include "Component/RsStaggerComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"
#include "Rs/AbilitySystem/Attributes/RsAttackSet.h"
#include "Rs/AbilitySystem/Attributes/RsDefenseSet.h"
#include "Rs/AbilitySystem/Attributes/RsEnergySet.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"
#include "Rs/AbilitySystem/Attributes/RsStaggerSet.h"
#include "Rs/Battle/RsBattleLibrary.h"


ARsCharacterBase::ARsCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<URsCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Avoid ticking characters if possible.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	GetCapsuleComponent()->InitCapsuleSize(40.f, 90.f);
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	// Set the pointer from Character Base to the Ability System Component sub-object.
	AbilitySystemComponent = CreateDefaultSubobject<URsAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	
	// These attribute sets will be detected by AbilitySystemComponent::InitializeComponent. Keeping a reference so that the sets don't get garbage collected before that.
	HealthSet = CreateDefaultSubobject<URsHealthSet>(TEXT("HealthSet"));
	StaggerSet = CreateDefaultSubobject<URsStaggerSet>(TEXT("StaggerSet"));
	EnergySet = CreateDefaultSubobject<URsEnergySet>(TEXT("EnergySet"));
	AttackSet = CreateDefaultSubobject<URsAttackSet>(TEXT("AttackSet"));
	DefenseSet = CreateDefaultSubobject<URsDefenseSet>(TEXT("DefenseSet"));
	
	HealthComponent = CreateDefaultSubobject<URsHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDeathStarted.AddDynamic(this, &ThisClass::HandleDeathStarted);
	
	StaggerComponent = CreateDefaultSubobject<URsStaggerComponent>(TEXT("StaggerComponent"));
	StaggerComponent->OnGroggyStarted.AddDynamic(this, &ThisClass::HandleGroggyStarted);

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	//GetMesh()->SetVisibility(false);
	
	RagdollComponent = CreateDefaultSubobject<URsRagdollComponent>(TEXT("RagdollComponent"));
	
	CharacterAppearance = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterAppearance"));
	CharacterAppearance->SetupAttachment(GetMesh());
	CharacterAppearance->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	CharacterAppearance->SetCollisionProfileName(TEXT("CharacterMesh"));

	BattleActorManagerComponent = CreateDefaultSubobject<URsBattleActorManagerComponent>(TEXT("BattleActorManagerComponent"));
	
	// AbilitySystemComponent needs to be updated at a high frequency.
	SetNetUpdateFrequency(100.0f);
	bReplicates = true;
	bAlwaysRelevant = true;
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
	check(AbilitySystemComponent);
	FGameplayTagContainer OwnedTags;
	AbilitySystemComponent->GetOwnedGameplayTags(OwnedTags);
	OutTagContainer = MoveTemp(OwnedTags);
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

void ARsCharacterBase::HandleDeathStarted(AActor* OwningActor)
{
	if (GetController())
	{
		GetController()->SetIgnoreMoveInput(true);
	}

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	UCharacterMovementComponent* CharMoveComp = GetCharacterMovement();
	check(CharMoveComp);
	CharMoveComp->StopMovementImmediately();
	CharMoveComp->DisableMovement();
	
	RagdollComponent->StartRagdoll_Local();
}

void ARsCharacterBase::HandleGroggyStarted(AActor* OwningActor)
{
	
}
