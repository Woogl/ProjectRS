// Copyright 2024 Team BH.


#include "RsProjectile.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Rs/AbilitySystem/RsAbilitySystemGlobals.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"
#include "Rs/Targeting/RsTargetingLibrary.h"

ARsProjectile::ARsProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HandleBeginOverlap);
	Collision->OnComponentHit.AddDynamic(this, &ThisClass::HandleBlock);
	Collision->SetCollisionProfileName(TEXT("RsProjectile"));
	SetRootComponent(Collision);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	
	Arrow = CreateDefaultSubobject<UArrowComponent>("Arrow");
	Arrow->SetupAttachment(GetRootComponent());
	
	InitialLifeSpan = 10.f;
}

bool ARsProjectile::ApplyEffect(AActor* Target, const FHitResult& HitResult)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);
	if (!InstigatorASC || !TargetASC)
	{
		return false;
	}
	
	bool bSuccess = false;
	for (FGameplayEffectSpecHandle& EffectSpec : EffectSpecs)
	{
		FGameplayEffectSpec* GESpec = EffectSpec.Data.Get();
		GESpec->GetContext().AddHitResult(HitResult);
		FActiveGameplayEffectHandle Handle = InstigatorASC->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data, TargetASC);
		if (Handle.IsValid() || GESpec->Def->DurationPolicy == EGameplayEffectDurationType::Instant)
		{
			bSuccess = true;
		}
	}
	return bSuccess;
}

void ARsProjectile::BeginPlay()
{
	Super::BeginPlay();

	InstigatorASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetInstigator());
	if (!InstigatorASC)
	{
		return;
	}
	
	FGameplayEffectContextHandle EffectContext = InstigatorASC->MakeEffectContext();
	int32 Level = OwningAbility ? OwningAbility->GetAbilityLevel() : 0;
	FGameplayEffectSpecHandle GESpec = InstigatorASC->MakeOutgoingSpec(Effect, Level, EffectContext);
	if (GESpec.IsValid())
	{
		EffectSpecs.Add(GESpec);
	}
	
	if (const FRsEffectTableRowBase* TableRow = EffectTableRow.GetRow<FRsEffectTableRowBase>(ANSI_TO_TCHAR(__FUNCTION__)))
	{
		const TSubclassOf<UGameplayEffect> EffectClass = TableRow->EffectClass;
		FGameplayEffectSpecHandle TableGESpec = InstigatorASC->MakeOutgoingSpec(EffectClass, 0, EffectContext);
		if (TableGESpec.IsValid())
		{
			// Set table data in GE spec
			URsAbilitySystemGlobals::SetSetByCallerTableRowHandle(*TableGESpec.Data, &EffectTableRow);
			EffectSpecs.Add(TableGESpec);
		}
	}
}

void ARsProjectile::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!InstigatorASC)
	{
		return;
	}
	
	TArray<AActor*> InActors;
	InActors.Add(OtherActor);
	TArray<AActor*> FilteredActor = URsTargetingLibrary::PerformFiltering(InActors, GetInstigator(), EffectFilter);
	
	if (!FilteredActor.Contains(OtherActor))
	{
		return;
	}

	bool bSuccess = ApplyEffect(OtherActor, SweepResult);
	if (!bSuccess)
	{
		return;
	}

	if (!HasAuthority())
	{
		return;
	}
	
	if (MaxEffectApplyCounts--)
	{
		Destroy();
	}
}

void ARsProjectile::HandleBlock(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetInstigator());
	if (!InstigatorASC || !TargetASC)
	{
		return;
	}
	
	TArray<AActor*> InActors;
	InActors.Add(OtherActor);
	TArray<AActor*> FilteredActor = URsTargetingLibrary::PerformFiltering(InActors, GetInstigator(), EffectFilter);
	
	if (FilteredActor.Contains(OtherActor))
	{
		ApplyEffect(OtherActor, Hit);
	}
	
	if (HasAuthority())
	{
		Destroy();
	}
}