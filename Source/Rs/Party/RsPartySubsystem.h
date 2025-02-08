// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "RsPartySubsystem.generated.h"

class ARsPlayerCharacter;
/**
 * 
 */
UCLASS()
class RS_API URsPartySubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	static URsPartySubsystem* Get(ULocalPlayer* LocalPlayer);
	
	void AddPartyMemberClass(TSubclassOf<ARsPlayerCharacter> NewMemberClass);
	void RemovePartyMemberClass(TSubclassOf<ARsPlayerCharacter> MemberClassToRemove);

	TSet<TSubclassOf<ARsPlayerCharacter>> GetPartyMemberClasses() const;

private:
	UPROPERTY()
	TSet<TSubclassOf<ARsPlayerCharacter>> PartyMemberClasses;
};
