// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RsPartyLibrary.generated.h"

class ARsPlayerCharacter;
/**
 * 
 */
UCLASS()
class RS_API URsPartyLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Rs Party Library", meta = (WorldContext = "WorldContextObject"))
	static void SwitchPartyMember(UObject* WorldContextObject, int32 NewMemberIndex);

	UFUNCTION(BlueprintCallable, Category = "Rs Party Library")
	static void AddPartyMember(ARsPlayerCharacter* NewMember);

	UFUNCTION(BlueprintCallable, Category = "Rs Party Library")
	static void InsertPartyMember(ARsPlayerCharacter* NewMember, int32 MemberIndex);
	
	UFUNCTION(BlueprintCallable, Category = "Rs Party Library")
	static void RemovePartyMember(ARsPlayerCharacter* MemberToRemove);

	UFUNCTION(BlueprintCallable, Category = "Rs Party Library")
	static void AddPartyMemberClass(ULocalPlayer* LocalPlayer, TSubclassOf<ARsPlayerCharacter> NewMemberClass);
	
	UFUNCTION(BlueprintCallable, Category = "Rs Party Library")
	static void RemovePartyMemberClass(ULocalPlayer* LocalPlayer, TSubclassOf<ARsPlayerCharacter> MemberClassToRemove);
};
