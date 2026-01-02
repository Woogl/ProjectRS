// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "GameplayEffect.h"
#include "Effect/RsEffectTable.h"
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
	
	// Used in global effect table for dereferencing table row.
	static void SetEffectTableRowHandle(FGameplayEffectContext& OutContext, const FDataTableRowHandle* RowHandle);
	
	static FDataTableRowHandle GetEffectTableRowHandle(FGameplayEffectContextHandle ContextHandle);
	
	template <typename T>
	static const T* GetEffectTableRow(FGameplayEffectContextHandle ContextHandle);
	
private:
	static UDataTable* GetOrLoadEffectTable(int16 TableIndex);
	
	UPROPERTY(Transient)
	TArray<UDataTable*> CachedEffectTables;
};

template <typename T>
const T* URsAbilitySystemGlobals::GetEffectTableRow(FGameplayEffectContextHandle ContextHandle)
{
	static_assert(TIsDerivedFrom<T, FRsEffectTableRowBase>::IsDerived, "T must be derived from FRsEffectTableRowBase");
	FDataTableRowHandle EffectTableRow = GetEffectTableRowHandle(ContextHandle);
	return EffectTableRow.GetRow<T>(ANSI_TO_TCHAR(__FUNCTION__));
}
