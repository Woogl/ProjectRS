// Copyright 2024 Team BH.


#include "RsAbilitySystemGlobals.h"

#include "GameplayEffect.h"
#include "Effect/RsGameplayEffectContext.h"
#include "Rs/RsLogChannels.h"

FGameplayEffectContext* URsAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FRsGameplayEffectContext();
}

void URsAbilitySystemGlobals::SetSetByCallerTableRowHandle(FGameplayEffectSpec& OutSpec, const FDataTableRowHandle* RowHandle)
{
	if (!RowHandle->DataTable)
	{
		UE_LOG(LogRsAbility, Warning, TEXT("Invalid table: %s"), *RowHandle->ToDebugString());
		return;
	}
	const FString TablePath = RowHandle->DataTable.GetPath();
	const int32 TableRowIndex = RowHandle->DataTable->GetRowNames().IndexOfByKey(RowHandle->RowName);
	if (TableRowIndex == INDEX_NONE)
	{
		UE_LOG(LogRsAbility, Warning, TEXT("Invalid table row: %s"), *RowHandle->ToDebugString());
		return;
	}
	OutSpec.SetSetByCallerMagnitude(FName(TablePath), TableRowIndex);
}

FDataTableRowHandle URsAbilitySystemGlobals::GetSetByCallerTableRowHandle(const FGameplayEffectSpec& Spec)
{
	for (const TTuple<FName, float>& SetByCallerData : Spec.SetByCallerNameMagnitudes)
	{
		const FName DataTablePath = SetByCallerData.Key;
		const int32 RowIndex = SetByCallerData.Value;
		if (const UDataTable* DataTable = LoadObject<UDataTable>(nullptr, *DataTablePath.ToString()))
		{
			TArray<FName> RowNames = DataTable->GetRowNames();
			if (RowNames.IsValidIndex(RowIndex))
			{
				FDataTableRowHandle RowHandle;
				RowHandle.DataTable = DataTable;
				RowHandle.RowName = RowNames[RowIndex];
				return RowHandle;
			}
		}
	}
	return FDataTableRowHandle();
}
