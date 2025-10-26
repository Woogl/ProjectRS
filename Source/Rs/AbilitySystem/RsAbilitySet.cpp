// Copyright 2024 Team BH.


#include "RsAbilitySet.h"

#include "Attributes/RsAttributeTableRow.h"

#if WITH_EDITOR
void URsAbilitySet::PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	FName MemberPropertyName = PropertyChangedEvent.GetMemberPropertyName();
	if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(FDataTableRowHandle, DataTable))
	{
		GrantedAttributes.Empty();
		
		const FName AssetName = FName(GetNameSafe(this));
		if (GrantedAttributeTableRow.DataTable->FindRow<FRsAttributeTableRow>(AssetName, ANSI_TO_TCHAR(__FUNCTION__)))
		{
			GrantedAttributeTableRow.RowName = AssetName;
		}
	}
}

bool URsAbilitySet::CanEditChange(const FProperty* InProperty) const
{
	bool bParentVal = Super::CanEditChange(InProperty);
	
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, GrantedAttributes))
	{
		bool bIsDataTableNull = GrantedAttributeTableRow.IsNull();
		return bParentVal && bIsDataTableNull;
	}
	return bParentVal;
}
#endif // WITH_EDITOR
