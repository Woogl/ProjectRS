// Copyright 2024 Team BH.


#include "RsAbilitySet.h"

#include "Attributes/RsAttributeSetBase.h"

#if WITH_EDITOR
void URsAbilitySet::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	FName MemberPropertyName = PropertyChangedEvent.GetMemberPropertyName();
	if (MemberPropertyName == TEXT("DataTable"))
	{
		const FName AssetName = FName(GetNameSafe(this));
		if (GrantedAttributeTableRow.DataTable->FindRow<FRsAttributeMetaData>(AssetName, ANSI_TO_TCHAR(__FUNCTION__)))
		{
			GrantedAttributeTableRow.RowName = AssetName;
		}
	}
}
#endif // WITH_EDITOR