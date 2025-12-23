// Copyright 2025 Team BH.


#include "RsStatQuery.h"

#include "AbilitySystemComponent.h"
#include "Rs/RsLogChannels.h"
#include "Rs/AbilitySystem/RsAbilitySystemLibrary.h"

bool URsStatQueryLibrary::MatchesStatQuery(UAbilitySystemComponent* ASC, FRsStatQuery StatQuery)
{
	FGameplayAttribute StatAttribute = URsAbilitySystemLibrary::GetAttributeByTag(StatQuery.StatTag);
	if (!StatAttribute.IsValid())
	{
		UE_LOG(LogRsAbility, Error, TEXT("Invalid stat tag : %s"), *StatQuery.StatTag.ToString());
		return false;
	}
	if (!ASC->HasAttributeSetForAttribute(StatAttribute))
	{
		UE_LOG(LogRsAbility, Error, TEXT("%s doesn't have %s stat"), *ASC->GetOwnerActor()->GetActorLabel(), *StatQuery.StatTag.ToString());
		return false;
	}
	
	const float NumericStat = URsAbilitySystemLibrary::GetNumericAttributeByTag(ASC, StatQuery.StatTag);
	const float CompareValue = StatQuery.CompareValue;
	
	switch (StatQuery.Operator)
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
