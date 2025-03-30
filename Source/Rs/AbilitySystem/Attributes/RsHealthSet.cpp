// Copyright 2024 Team BH.


#include "RsHealthSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "Rs/RsLogChannels.h"

URsHealthSet::URsHealthSet()
{
	MaxHealth = 0.0f;
	CurrentHealth = 0.0f;
	HealthRegen = 0.0f;
	Shield = 0.0f;
}

void URsHealthSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_None;
	
	// Replicated to all
	DOREPLIFETIME_WITH_PARAMS_FAST(URsHealthSet, CurrentHealth, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(URsHealthSet, MaxHealth, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(URsHealthSet, Shield, Params);

	// Only Owner
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(URsHealthSet, HealthRegen, Params);
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
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}

	else if (Attribute == GetShieldAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
}

void URsHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthDamageAttribute())
	{
		// Store a local copy of the amount of Damage done and clear the Damage attribute.
		float LocalDamageDone = GetHealthDamage();
		SetHealthDamage(0.f);
	
		if (LocalDamageDone > 0.0f)
		{
			// Apply the Health change and then clamp it.
			if (GetShield() > 0.f)
			{
				const float LocalShield = GetShield();
				float Absorbed = FMath::Min(LocalDamageDone, LocalShield);
				SetShield(LocalShield - Absorbed);
				LocalDamageDone -= Absorbed;
			}

			if (LocalDamageDone > 0.f)
			{
				const float NewHealth = GetCurrentHealth() - LocalDamageDone;
				SetCurrentHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
			}
		}
	}

	else if (Data.EvaluatedData.Attribute == GetHealingAttribute())
	{
		// Store a local copy of the amount of Healing done and clear the Healing attribute.
		const float LocalHealingDone = GetHealing();
		SetHealing(0.f);

		if (FMath::IsNearlyZero(LocalHealingDone) == false)
		{
			// Apply the Health change and then clamp it.
			const float NewHealth = GetCurrentHealth() + LocalHealingDone;
			SetCurrentHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
		}
	}
	
	else if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth(), 0.0f, GetMaxHealth()));
	}

	else if (Data.EvaluatedData.Attribute == GetHealthRegenAttribute())
	{
		SetHealthRegen(FMath::Clamp(GetHealthRegen(), -GetMaxHealth(), GetMaxHealth()));
	}

	else if (Data.EvaluatedData.Attribute == GetShieldAttribute())
	{
		SetShield(FMath::Max(0.f, GetShield()));
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

void URsHealthSet::OnRep_HealthRegen(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsHealthSet, HealthRegen, OldValue);
}

void URsHealthSet::OnRep_Shield(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsHealthSet, Shield, OldValue);
}
