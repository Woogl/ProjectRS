// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RsPartyLibrary.generated.h"

struct FGameplayEffectSpec;
class UGameplayEffect;
class ARsPlayerCharacter;
/**
 * 
 */
UCLASS()
class RS_API URsPartyLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	/** Party member change */
	
	UFUNCTION(BlueprintCallable, Category = "Rs Party Library", meta = (WorldContext = "WorldContextObject"))
	static bool SwitchPartyMember(UObject* WorldContextObject, int32 NewMemberIndex);

	UFUNCTION(BlueprintCallable, Category = "Rs Party Library")
	static void AddPartyMember(ARsPlayerCharacter* NewMember);
	
	UFUNCTION(BlueprintCallable, Category = "Rs Party Library")
	static void RemovePartyMember(ARsPlayerCharacter* MemberToRemove);

	UFUNCTION(BlueprintCallable, Category = "Rs Party Library")
	static void InsertPartyMemberAt(ARsPlayerCharacter* NewMember, int32 MemberIndex);

	UFUNCTION(BlueprintCallable, Category = "Rs Party Library", meta = (WorldContext = "WorldContextObject"))
	static void RemovePartyMemberAt(UObject* WorldContextObject, int32 MemberIndex);

	UFUNCTION(BlueprintCallable, Category = "Rs Party Library")
	static void JoinPartyMember(ULocalPlayer* LocalPlayer, TSubclassOf<ARsPlayerCharacter> NewMemberClass);
	
	UFUNCTION(BlueprintCallable, Category = "Rs Party Library")
	static void LeavePartyMember(ULocalPlayer* LocalPlayer, TSubclassOf<ARsPlayerCharacter> MemberClassToRemove);

	/** Global party member effect */

	UFUNCTION(BlueprintCallable, Category = "Rs Party Library", meta = (WorldContext = "WorldContextObject"))
	static void ApplyPartyEffect(UObject* WorldContextObject, TSubclassOf<UGameplayEffect> EffectClass);

	UFUNCTION(BlueprintCallable, Category = "Rs Party Library", meta = (WorldContext = "WorldContextObject"))
	static void ApplyPartyEffectSpec(UObject* WorldContextObject, FGameplayEffectSpec EffectSpec);

	/** Global party member ability */

	UFUNCTION(BlueprintCallable, Category = "Rs Party Library", meta = (WorldContext = "WorldContextObject"))
	static void ActivatePartyAbility(UObject* WorldContextObject, FGameplayTag AbilityTag);

	UFUNCTION(BlueprintCallable, Category = "Rs Party Library", meta = (WorldContext = "WorldContextObject"))
	static void CancelPartyAbility(UObject* WorldContextObject, FGameplayTag AbilityTag);
};
