// Copyright 2025 Team BH.


#include "RsStatQuery.h"

#include "AbilitySystemComponent.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"

bool FRsStatQuery::IsValid() const
{
	return StatTag != FGameplayTag::EmptyTag;
}

bool FRsStatQuery::MatchesQuery(UAbilitySystemComponent* ASC) const
{
	FGameplayAttribute StatAttribute = URsAbilitySystemLibrary::GetAttributeByTag(StatTag);
	if (!StatAttribute.IsValid())
	{
		UE_LOG(LogRsAbility, Error, TEXT("Invalid stat tag : %s"), *StatTag.ToString());
		return false;
	}
	if (!ASC->HasAttributeSetForAttribute(StatAttribute))
	{
		UE_LOG(LogRsAbility, Error, TEXT("%s doesn't have %s stat"), *ASC->GetOwnerActor()->GetActorLabel(), *StatTag.ToString());
		return false;
	}
	
	const float NumericStat = URsAbilitySystemLibrary::GetNumericAttributeByTag(ASC, StatTag);
	switch (Operator)
	{
	case ERsStatRelationalOperator::Greater:
		return NumericStat > CompareValue;
	case ERsStatRelationalOperator::GreaterEqual:
		return NumericStat >= CompareValue;
	case ERsStatRelationalOperator::EqualEqual:
		return NumericStat == CompareValue;
	case ERsStatRelationalOperator::NotEqual:
		return NumericStat != CompareValue;
	case ERsStatRelationalOperator::LessEqual:
		return NumericStat <= CompareValue;
	case ERsStatRelationalOperator::Less:
		return NumericStat < CompareValue;
	default:
		break;
	}
	
	return false;
}

bool URsStatQueryLibrary::MatchesStatQuery(UAbilitySystemComponent* ASC, FRsStatQuery StatQuery)
{
	return StatQuery.MatchesQuery(ASC);
}
