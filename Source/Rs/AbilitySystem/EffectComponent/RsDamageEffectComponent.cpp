// Copyright 2025 Team BH.


#include "RsDamageEffectComponent.h"

#include "GameplayEffect.h"
#include "Misc/DataValidation.h"
#include "Rs/RsGameplayTags.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"
#include "Rs/AbilitySystem/Effect/RsEffectTable.h"
#include "Rs/AbilitySystem/Effect/RsGameplayEffect.h"


URsDamageEffectComponent::URsDamageEffectComponent()
{
	if (UGameplayEffect* Owner = GetOwner())
	{
		FString DefaultRowName = Owner->GetName();
		DefaultRowName.RemoveFromStart(TEXT("Default__"));
		DefaultRowName.RemoveFromEnd(TEXT("_C"));
		DataTableRow.RowName = FName(DefaultRowName);
	}

	HealthDamageCoefficients.Add(RsGameplayTags::COEFFICIENT_ATK_SOURCE, 1.f);
	StaggerDamageCoefficients.Add(RsGameplayTags::COEFFICIENT_IMP_SOURCE, 1.f);
	DamageTags.AddTag(RsGameplayTags::EFFECT_DAMAGE);
}

void URsDamageEffectComponent::OnGameplayEffectChanged()
{
	Super::OnGameplayEffectChanged();
	
	UGameplayEffect* Owner = GetOwner();
	if (const FRsDamageTableRow* DamageTableRow = GetDamageTableRow())
	{
		FGameplayTagContainer TableDamageTags;
		TableDamageTags.AddTag(DamageTableRow->DamageTag1);
		TableDamageTags.AddTag(DamageTableRow->DamageTag2);
		Owner->CachedAssetTags.AppendTags(TableDamageTags);
	}
	else
	{
		Owner->CachedAssetTags.AppendTags(DamageTags);
	}
}

bool URsDamageEffectComponent::CanGameplayEffectApply(const FActiveGameplayEffectsContainer& ActiveGEContainer, const FGameplayEffectSpec& GESpec) const
{
	float StatValue = URsAbilitySystemLibrary::GetNumericAttributeByTag(ActiveGEContainer.Owner, RsGameplayTags::STAT_INV);
	if (const FRsDamageTableRow* DamageTableRow = GetDamageTableRow())
	{
		return StatValue <= DamageTableRow->InvinciblePierce;
	}
	else
	{
		return StatValue <= InvinciblePierce;
	}
}

#if WITH_EDITOR
bool URsDamageEffectComponent::CanEditChange(const FProperty* InProperty) const
{
	bool bParentVal = Super::CanEditChange(InProperty);

	if (DataTableRow.DataTable)
	{
		FName PropertyName = InProperty->GetFName();
		if (PropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, HealthDamageCoefficients))
		{
			return false;
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, StaggerDamageCoefficients))
		{
			return false;
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, InvinciblePierce))
		{
			return false;
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, SuperArmorPierce))
		{
			return false;
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, HitReaction))
		{
			return false;
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, SourceHitStopTime))
		{
			return false;
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, TargetHitStopTime))
		{
			return false;
		}
		if (PropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, DamageTags))
		{
			return false;
		}
	}
	return bParentVal;
}

void URsDamageEffectComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName MemberPropertyName = PropertyChangedEvent.GetMemberPropertyName();
	if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, DataTableRow))
	{
		HealthDamageCoefficients.Empty();
		StaggerDamageCoefficients.Empty();
		InvinciblePierce = 0;
		SuperArmorPierce = 0;
		HitReaction = FGameplayTag::EmptyTag;
		SourceHitStopTime = 0.f;
		TargetHitStopTime = 0.f;
		DamageTags = FGameplayTagContainer::EmptyContainer;
	}
}

EDataValidationResult URsDamageEffectComponent::IsDataValid(class FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	// Check GE
	if (GetOuter() && !GetOuter()->IsA(URsGameplayEffect::StaticClass()))
	{
		Context.AddError(FText::FromString(FString::Printf(TEXT("RsDamageEffectComponent supports RsGameplayEffect only."))));
		return EDataValidationResult::Invalid;
	}

	// Check asset data
	if (!DataTableRow.DataTable)
	{
		for (const auto& [CoeffTag, CoeffNum] : HealthDamageCoefficients)
		{
			if (CoeffTag == RsGameplayTags::COEFFICIENT_CONSTANT)
			{
				continue;
			}
			FString CoeffTagString = CoeffTag.ToString();
			if (!CoeffTagString.StartsWith(TEXT("Coefficient.")))
			{
				Context.AddError(FText::FromString(FString::Printf(TEXT("Coefficient tag { %s } must start with \"Coefficient.\""), *CoeffTag.ToString())));
				return EDataValidationResult::Invalid;
			}
			if (!CoeffTagString.EndsWith(TEXT(".Source")) && !CoeffTagString.EndsWith(TEXT(".Target")))
			{
				Context.AddError(FText::FromString(FString::Printf(TEXT("Coefficient tag { %s } must end with \".Target\" or \".Source\""), *CoeffTag.ToString())));
				return EDataValidationResult::Invalid;
			}
		}

		for (const auto& [CoeffTag, CoeffNum] : StaggerDamageCoefficients)
		{
			if (CoeffTag == RsGameplayTags::COEFFICIENT_CONSTANT)
			{
				continue;
			}
			FString CoeffTagString = CoeffTag.ToString();
			if (CoeffTagString.IsEmpty())
			{
				Context.AddError(FText::FromString(FString::Printf(TEXT("Coefficient tag is empty"))));
				return EDataValidationResult::Invalid;
			}
			if (!CoeffTagString.StartsWith(TEXT("Coefficient.")))
			{
				Context.AddError(FText::FromString(FString::Printf(TEXT("Coefficient tag \"%s\" must start with \"Coefficient.\""), *CoeffTag.ToString())));
				return EDataValidationResult::Invalid;
			}
			if (!CoeffTagString.EndsWith(TEXT(".Source")) && !CoeffTagString.EndsWith(TEXT(".Target")))
			{
				Context.AddError(FText::FromString(FString::Printf(TEXT("Coefficient tag \"%s\" must end with \".Target\" or \".Source\""), *CoeffTag.ToString())));
				return EDataValidationResult::Invalid;
			}
		}
		return Result;
	}

	// Check table data
	const FRsDamageTableRow* Row = GetDamageTableRow();
	if (!Row)
	{
		Context.AddError(FText::FromString(FString::Printf(TEXT("%s"), *DataTableRow.ToDebugString())));
		return EDataValidationResult::Invalid;
	}
	if (Row->IsDataValid(Context) == EDataValidationResult::Invalid)
	{
		Context.AddError(FText::FromString(FString::Printf(TEXT("%s"), *DataTableRow.ToDebugString())));
		return EDataValidationResult::Invalid;
	}
	return Result;
}
#endif // WITH_EDITOR

const FRsDamageTableRow* URsDamageEffectComponent::GetDamageTableRow() const
{
	if (!DataTableRow.DataTable || DataTableRow.RowName.IsNone())
	{
		return nullptr;
	}
	return DataTableRow.GetRow<FRsDamageTableRow>(TEXT(__FUNCTION__));
}
