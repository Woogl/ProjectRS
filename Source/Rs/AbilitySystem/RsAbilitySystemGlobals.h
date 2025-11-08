// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "GameplayEffect.h"
#include "RsAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class RS_API URsAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	/** Should allocate a project specific GameplayEffectContext struct. Caller is responsible for deallocation */
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;

	// Used in shared gameplay effect for dereferencing table row.
	static void SetSetByCallerTableRow(FGameplayEffectSpec& Spec, FDataTableRowHandle* RowHandle);
	
	template <typename T>
	static const T* GetSetByCallerTableRow(const FGameplayEffectSpec& Spec);
};

template <typename T>
const T* URsAbilitySystemGlobals::GetSetByCallerTableRow(const FGameplayEffectSpec& Spec)
{
	for (const TTuple<FName, float>& SetByCallerData : Spec.SetByCallerNameMagnitudes)
	{
		FName DataTablePath = SetByCallerData.Key;
		int32 RowIndex = SetByCallerData.Value;
		if (const UDataTable* DataTable = LoadObject<UDataTable>(nullptr, *DataTablePath.ToString()))
		{
			TArray<FName> RowNames = DataTable->GetRowNames();
			if (RowNames.IsValidIndex(RowIndex))
			{
				if (T* Row = DataTable->FindRow<T>(RowNames[RowIndex], ANSI_TO_TCHAR(__FUNCTION__)))
				{
					return Row;
				}
			}
		}
	}
	return nullptr;
}
