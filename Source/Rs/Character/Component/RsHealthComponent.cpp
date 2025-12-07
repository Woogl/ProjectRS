// Copyright 2024 Team BH.


#include "RsHealthComponent.h"

#include "AbilitySystemGlobals.h"
#include "GameplayEffectExtension.h"
#include "RsRagdollComponent.h"
#include "Net/UnrealNetwork.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AbilitySystem/Attributes/RsHealthSet.h"

URsHealthComponent::URsHealthComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void URsHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (URsAbilitySystemComponent* RsASC = URsAbilitySystemComponent::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		RsASC->CallOrRegister_OnAbilitySystemInitialized(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::HandleAbilitySystemInitialized));
	}
}

void URsHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URsHealthComponent, bIsDead);
}

void URsHealthComponent::Initialize(UAbilitySystemComponent* AbilitySystemComponent)
{
	if (!AbilitySystemComponent)
	{
		return;
	}
	
	HealthSet = AbilitySystemComponent->GetSet<URsHealthSet>();
	if (!HealthSet)
	{
		UE_LOG(RsAbilityLog, Error, TEXT("Cannot initialize RsHealthComponent for owner [%s] with NULL health set."), *GetNameSafe(GetOwner()));
		return;
	}
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetCurrentHealthAttribute()).AddUObject(this, &ThisClass::HandleHealthChange);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(URsHealthSet::GetBarrierAttribute()).AddUObject(this, &ThisClass::HandleBarrierChange);
	
	// HealthSet->OnCurrentHealthChanged.AddUObject(this, &ThisClass::HandleCurrentHealthChange);
	// HealthSet->OnMaxHealthChanged.AddUObject(this, &ThisClass::HandleMaxHealthChange);
	// HealthSet->OnOutOfHealth.AddUObject(this, &ThisClass::HandleOutOfHealth);
	
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &ThisClass::HandleBarrierAdded);
}

void URsHealthComponent::ApplyDamageToBarriers(UAbilitySystemComponent* AbilitySystemComponent, float DamageAmount)
{
	// DamageAmount == value between Input Damage ~ Max Shield (Clamp already done in HealthSet)
	for (const FActiveGameplayEffectHandle& ShieldHandle : ActiveBarrierHandles)
	{
		float ShieldAmount = AbilitySystemComponent->GetGameplayEffectMagnitude(ShieldHandle, URsHealthSet::GetBarrierAttribute());
		float AccumulatedDamage = FMath::Abs(AbilitySystemComponent->GetNumericAttributeBase(URsHealthSet::GetBarrierAttribute()));
		float ShieldDamage = FMath::Min(DamageAmount,ShieldAmount - AccumulatedDamage);

		// Accumulate current handled Damage amount in BaseValue of Shield Attribute
		AbilitySystemComponent->ApplyModToAttribute(URsHealthSet::GetBarrierAttribute(), EGameplayModOp::Additive, -ShieldDamage);
		DamageAmount -= ShieldDamage;

		// if Sum of Damage(for this shield) reached To ShieldAmount, it's broken
		const bool bIsShieldBroken = FMath::IsNearlyEqual(AccumulatedDamage + ShieldDamage,ShieldAmount);
		if (bIsShieldBroken)
		{
			HandleBarrierBroke(ShieldHandle);
		}
		// break loop when Damage is handled completely through shield
		if (FMath::IsNearlyZero(DamageAmount))
		{
			break;
		}
	}
	for (const FActiveGameplayEffectHandle& RemovePendingShieldHandle : RemovePendingBarrierHandles)
	{
		ActiveBarrierHandles.Remove(RemovePendingShieldHandle);	
	}
	RemovePendingBarrierHandles.Empty();
}

float URsHealthComponent::GetCurrentHealth()
{
	if (HealthSet)
	{
		return HealthSet->GetCurrentHealth();
	}
	return 0.f;
}

float URsHealthComponent::GetMaxHealth()
{
	if (HealthSet)
	{
		return HealthSet->GetMaxHealth();
	}
	return 0.f;
}

float URsHealthComponent::GetBarrier()
{
	if (HealthSet)
	{
		return HealthSet->GetBarrier();
	}
	return 0.f;
}

void URsHealthComponent::HandleAbilitySystemInitialized()
{
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		Initialize(ASC);
	}
}

void URsHealthComponent::HandleCurrentHealthChange(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	OnCurrentHealthChanged.Broadcast(this, OldValue, NewValue, DamageInstigator);
}

void URsHealthComponent::HandleMaxHealthChange(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
	OnMaxHealthChanged.Broadcast(this, OldValue, NewValue, DamageInstigator);
}

void URsHealthComponent::HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser, const FGameplayEffectSpec* DamageEffectSpec, float DamageMagnitude, float OldValue, float NewValue)
{
#if WITH_SERVER_CODE
	auto AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
	if (AbilitySystemComponent && DamageEffectSpec)
	{
		// Send the "GameplayEvent.Death" gameplay event through the owner's ability system.  This can be used to trigger a death gameplay ability.
		FGameplayEventData Payload;
		Payload.EventTag = RsGameplayTags::ABILITY_DEATH;
		Payload.Instigator = DamageInstigator;
		Payload.Target = AbilitySystemComponent->GetAvatarActor();
		Payload.OptionalObject = DamageEffectSpec->Def;
		Payload.ContextHandle = DamageEffectSpec->GetEffectContext();
		Payload.InstigatorTags = *DamageEffectSpec->CapturedSourceTags.GetAggregatedTags();
		Payload.TargetTags = *DamageEffectSpec->CapturedTargetTags.GetAggregatedTags();
		Payload.EventMagnitude = DamageMagnitude;

		FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
		AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
	}

#endif // #if WITH_SERVER_CODE
}

void URsHealthComponent::HandleHealthChange(const FOnAttributeChangeData& ChangeData)
{
	UE_LOG(LogTemp, Warning, TEXT("health changed: %f -> %f"), ChangeData.OldValue, ChangeData.NewValue);
	
	if (ChangeData.NewValue <= 0.f && bIsDead == false)
	{
		if (GetOwner()->HasAuthority())
		{
			const bool bOldDead = bIsDead;
			bIsDead = true;
			OnRep_bIsDead(bOldDead);
			GetOwner()->ForceNetUpdate();
		}
	}
	//HealthChange.Broadcast(ChangeData.OldValue, ChangeData.NewValue);
}

void URsHealthComponent::HandleBarrierChange(const FOnAttributeChangeData& ChangeData)
{
	//OnBarrierChange.Broadcast(ChangeData.OldValue, ChangeData.NewValue);
}

void URsHealthComponent::HandleBarrierAdded(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	bool bModifyBarrier = false;
	for (const FGameplayEffectModifiedAttribute& ModifiedAttribute : GESpec.ModifiedAttributes)
	{
		if (ModifiedAttribute.Attribute == URsHealthSet::GetBarrierAttribute())
		{
			bModifyBarrier = true;
			break;
		}
	}
	if (!bModifyBarrier)
	{
		return;
	}

	float BarrierMagnitude = AbilitySystemComponent->GetGameplayEffectMagnitude(ActiveEffectHandle, URsHealthSet::GetBarrierAttribute());
	AbilitySystemComponent->OnGameplayEffectRemoved_InfoDelegate(ActiveEffectHandle)->AddUObject(this, &ThisClass::HandleBarrierRemove, BarrierMagnitude);
		
	ActiveBarrierHandles.Add(ActiveEffectHandle);
	ActiveBarrierHandles.Sort([&AbilitySystemComponent](const FActiveGameplayEffectHandle& A, const FActiveGameplayEffectHandle& B)
	{
		const FActiveGameplayEffect* ABarrier = AbilitySystemComponent->GetActiveGameplayEffect(A);
		const FActiveGameplayEffect* BBarrier = AbilitySystemComponent->GetActiveGameplayEffect(B);
		if (ABarrier && BBarrier)
		{
			return ABarrier->GetEndTime() < BBarrier->GetEndTime();
		}
		return BBarrier != nullptr;;
	});
}

void URsHealthComponent::HandleBarrierRemove(const FGameplayEffectRemovalInfo& RemovalInfo, float Magnitude)
{
	if (!RemovePendingBarrierHandles.Contains(RemovalInfo.ActiveEffect->Handle))
	{
		ActiveBarrierHandles.Remove(RemovalInfo.ActiveEffect->Handle);
	}
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		ASC->ApplyModToAttribute(URsHealthSet::GetBarrierAttribute(), EGameplayModOp::Additive, Magnitude);
	}
}

void URsHealthComponent::HandleBarrierBroke(const FActiveGameplayEffectHandle& BrokenBarrierHandle)
{
	// Order of shield must be observed.
	RemovePendingBarrierHandles.Add(BrokenBarrierHandle);
	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		ASC->RemoveActiveGameplayEffect(BrokenBarrierHandle);
	}
}

void URsHealthComponent::OnRep_bIsDead(bool OldValue)
{
	if (OldValue == false && bIsDead == true)
	{
		// Handle death
		FGameplayEventData Payload;
		Payload.EventTag = RsGameplayTags::ABILITY_DEATH;
		if (UAbilitySystemComponent* OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
		{
			OwnerASC->HandleGameplayEvent(RsGameplayTags::ABILITY_DEATH, &Payload);
		}
		if (URsRagdollComponent* RagdollComp = GetOwner()->FindComponentByClass<URsRagdollComponent>())
		{
			RagdollComp->StartRagdoll_Local();
		}
		OnDeathStarted.Broadcast(GetOwner());
	}
}
