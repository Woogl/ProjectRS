// Copyright 2024 Team BH.


#include "RsHealthSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/Character/Component/RsHealthComponent.h"

URsHealthSet::URsHealthSet()
	: CurrentHealth(0.f)
	, MaxHealth(1.f)
	, Barrier(0.f)
{
	HealthDamageCueTag = RsGameplayTags::GAMEPLAYCUE_DAMAGE_HEALTH;
	HealingCueTag = RsGameplayTags::GAMEPLAYCUE_HEALING;
}

void URsHealthSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(URsHealthSet, CurrentHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URsHealthSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URsHealthSet, Barrier, COND_None, REPNOTIFY_Always);
}

void URsHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
	}
	else if (Attribute == GetCurrentHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetBarrierAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
}

void URsHealthSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(GetCurrentHealthAttribute(), OldValue, NewValue);
	}
}

void URsHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetFinalDamageAttribute())
	{
		float LocalFinalDamage = GetFinalDamage();
		SetBaseDamage(0.f);
		SetFinalDamage(0.f);
		if (LocalFinalDamage > 0.f)
		{
			if (GetBarrier() > 0.f)
			{
				if (URsHealthComponent* HealthComponent = GetActorInfo()->AvatarActor->FindComponentByClass<URsHealthComponent>())
				{
					float Absorbed = FMath::Min(GetBarrier(), LocalFinalDamage);
					LocalFinalDamage -= Absorbed;
					HealthComponent->ApplyDamageToBarriers(GetOwningAbilitySystemComponent(), Absorbed);
				}
			}
			SetCurrentHealth(FMath::Clamp(GetCurrentHealth() - LocalFinalDamage, 0.f, GetMaxHealth()));
		}

		FGameplayCueParameters GameplayCueParams;
		GameplayCueParams.EffectContext = Data.EffectSpec.GetEffectContext();
		GameplayCueParams.RawMagnitude = LocalFinalDamage;
		GetOwningAbilitySystemComponent()->ExecuteGameplayCue(HealthDamageCueTag, GameplayCueParams);
	}
	
	else if (Data.EvaluatedData.Attribute == GetHealingAttribute())
	{
		float LocalHealingDone = GetHealing();
		SetHealing(0.f);

		if (FMath::IsNearlyZero(LocalHealingDone) == false)
		{
			// Apply the Health change and then clamp it.
			const float NewHealth = GetCurrentHealth() + LocalHealingDone;
			SetCurrentHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
		}

		GetOwningAbilitySystemComponent()->ExecuteGameplayCue(HealingCueTag, Data.EffectSpec.GetEffectContext());
	}
	
	else if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth()));
	}
}

void URsHealthSet::OnRep_CurrentHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsHealthSet, CurrentHealth, OldValue);
}

void URsHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsHealthSet, MaxHealth, OldValue);
}

void URsHealthSet::OnRep_Barrier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsHealthSet, Barrier, OldValue);
}
