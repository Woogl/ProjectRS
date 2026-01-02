// Copyright 2024 Team BH.


#include "RsAbilitySystemGlobals.h"

#include "RsAbilitySystemSettings.h"
#include "Effect/RsGameplayEffectContext.h"
#include "Rs/RsLogChannels.h"

FGameplayEffectContext* URsAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FRsGameplayEffectContext();
}

void URsAbilitySystemGlobals::SetEffectTableRowHandle(FGameplayEffectContext& OutContext, const FDataTableRowHandle* RowHandle)
{
	FRsGameplayEffectContext* RsContext = static_cast<FRsGameplayEffectContext*>(&OutContext);
	if (!RowHandle || !RowHandle->DataTable || RowHandle->RowName.IsNone())
	{
		UE_LOG(LogRsAbility, Warning, TEXT("Invalid table row handle in effect context"));
		return;
	}
	const TSoftObjectPtr<UDataTable> SoftDataTable(FSoftObjectPath(RowHandle->DataTable));
	RsContext->EffectTableIndex = URsAbilitySystemSettings::Get().EffectTables.Find(SoftDataTable);
	RsContext->EffectRowName = RowHandle->RowName;
}

FDataTableRowHandle URsAbilitySystemGlobals::GetEffectTableRowHandle(FGameplayEffectContextHandle ContextHandle)
{
	if (!ContextHandle.IsValid())
	{
		return FDataTableRowHandle();
	}
	const FRsGameplayEffectContext* RsContext = static_cast<const FRsGameplayEffectContext*>(ContextHandle.Get());
	const int32 TableIndex = RsContext->EffectTableIndex;
	const TArray<TSoftObjectPtr<UDataTable>>& EffectTables = URsAbilitySystemSettings::Get().EffectTables;

	FDataTableRowHandle RowHandle;
	if (!EffectTables.IsValidIndex(TableIndex) || RsContext->EffectRowName.IsNone())
	{
		UE_LOG(LogRsAbility, Warning, TEXT("Cannot find effect table row handle from EffectContext"));
		return RowHandle;
	}

	UDataTable* EffectTable = EffectTables[TableIndex].Get();
	if (!EffectTable)
	{
		EffectTable = EffectTables[TableIndex].LoadSynchronous();
	}
	
	RowHandle.DataTable = EffectTable;
	RowHandle.RowName = RsContext->EffectRowName;
	return RowHandle;
}
