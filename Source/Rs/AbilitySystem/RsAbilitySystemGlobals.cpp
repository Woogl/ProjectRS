// Copyright 2024 Team BH.


#include "RsAbilitySystemGlobals.h"

#include "GameplayEffect.h"
#include "Effect/RsGameplayEffectContext.h"
#include "Rs/RsLogChannels.h"

FGameplayEffectContext* URsAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FRsGameplayEffectContext();
}

void URsAbilitySystemGlobals::SetSetByCallerTableRow(FGameplayEffectSpec& Spec, FDataTableRowHandle* RowHandle)
{
	if (!RowHandle->DataTable)
	{
		UE_LOG(RsLog, Warning, TEXT("Invalid table: %s"), *RowHandle->ToDebugString());
		return;
	}
	FString TablePath = RowHandle->DataTable.GetPath();
	int32 TableRowIndex = RowHandle->DataTable->GetRowNames().IndexOfByKey(RowHandle->RowName);
	if (TableRowIndex == INDEX_NONE)
	{
		UE_LOG(RsLog, Warning, TEXT("Invalid table row: %s"), *RowHandle->ToDebugString());
		return;
	}
	Spec.SetSetByCallerMagnitude(FName(TablePath), TableRowIndex);
}
