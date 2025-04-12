// Copyright 2024 Team BH.


#include "RsHealthSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/System/RsDeveloperSetting.h"

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

bool URsHealthSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	bool Result = Super::PreGameplayEffectExecute(Data);
	return Result;
}

void URsHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthDamageAttribute())
	{
		// Store a local copy of the amount of Damage done and clear the Damage attribute.
		float LocalDamageDone = GetHealthDamage();
   		SetHealthDamage(0.f);

		if (GetShield() > 0.f)
		{
			UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
			FGameplayTagContainer ShieldGETags(URsDeveloperSetting::Get()->ShieldEffectTag);
			FGameplayEffectQuery ShieldGEQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(ShieldGETags);
			TArray<FActiveGameplayEffectHandle> ShieldGEHandles = ASC->GetActiveEffects(ShieldGEQuery);
			TArray<const FActiveGameplayEffect*> ShieldGEs;
			// get shield GE
			for (FActiveGameplayEffectHandle ShieldGEHandle : ShieldGEHandles)
			{
				ShieldGEs.Add(ASC->GetActiveGameplayEffect(ShieldGEHandle));
			}
			// sort shields with endtime
			if (ShieldGEs.Num() > 1)
			{
				ShieldGEs.Sort([](const FActiveGameplayEffect& A, const FActiveGameplayEffect& B)->bool{return A.GetEndTime() < B.GetEndTime();});				
			}
			
			int32 ShieldIndex = 0;
			for (; ShieldIndex < ShieldGEs.Num(); ShieldIndex++)
			{
				// current shield's shield amount
				const float ShieldAmount = ShieldGEs[ShieldIndex]->Spec.GetModifierMagnitude(0,false);
				// current shield's accumulated Damage amount (Saved in BaseValue as negative float, convert to absolute value for use)
				const float CurDamage = FMath::Abs(GetShieldAttribute().GetGameplayAttributeData(this)->GetBaseValue());
				// current shield-acceptable Damage amount
				const float Absorbed = FMath::Min(LocalDamageDone, ShieldAmount - CurDamage);

				// accumulate damage
				ASC->ApplyModToAttribute(GetShieldAttribute(),EGameplayModOp::Additive, -Absorbed);
				LocalDamageDone -= Absorbed;

				const bool bIsShieldBroken = FMath::IsNearlyEqual(CurDamage + Absorbed,ShieldAmount);
				if (bIsShieldBroken)
				{
					// reset damage amount to 0
					SetShield(0.f);
					// remove shield
					ASC->RemoveActiveGameplayEffect(ShieldGEs[ShieldIndex]->Handle);
					ShieldGEs.RemoveAt(ShieldIndex);
					ShieldIndex--;
				}
				// break loop when Damage is handled completely through shield
				if (FMath::IsNearlyZero(LocalDamageDone))
				{
					break;
				}
			}
		}
		
		if (LocalDamageDone > 0.0f)
		{
			SetCurrentHealth(FMath::Clamp(GetCurrentHealth() - LocalDamageDone, 0.0f, GetMaxHealth()));
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
