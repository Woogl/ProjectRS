// Copyright 2024 Team BH.


#include "RsHealthSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "Rs/Character/Component/RsHealthComponent.h"

URsHealthSet::URsHealthSet()
{
	MaxHealth = 1.f;
	CurrentHealth = 0.f;
	HealthLoss = 0.f;
	Barrier = 0.f;
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
	DOREPLIFETIME_WITH_PARAMS_FAST(URsHealthSet, Barrier, Params);

	// Only Owner
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(URsHealthSet, HealthLoss, Params);
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

	if (Data.EvaluatedData.Attribute == GetHealthDamageAttribute())
	{
		float LocalHealthDamage = GetHealthDamage();
		if (LocalHealthDamage > 0.f)
		{
			if (GetBarrier() > 0.f)
			{
				if (URsHealthComponent* HealthComponent = GetActorInfo()->AvatarActor->FindComponentByClass<URsHealthComponent>())
				{
					float Absorbed = FMath::Min(GetBarrier(), LocalHealthDamage);
					LocalHealthDamage -= Absorbed;
					HealthComponent->ApplyDamageToBarriers(GetOwningAbilitySystemComponent(), Absorbed);
				}
			}
			SetCurrentHealth(FMath::Clamp(GetCurrentHealth() - LocalHealthDamage, 0.f, GetMaxHealth()));
		}
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
	}
	
	else if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth()));
		SetHealthLoss(FMath::Clamp(GetMaxHealth() - GetCurrentHealth(), 0.f, GetMaxHealth()));
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

void URsHealthSet::OnRep_HealthLoss(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsHealthSet, HealthLoss, OldValue);
}

void URsHealthSet::OnRep_Barrier(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(URsHealthSet, Barrier, OldValue);
}
