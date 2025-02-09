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
	
	void JoinPartyMember(TSubclassOf<ARsPlayerCharacter> NewMember);
	void LeavePartyMember(TSubclassOf<ARsPlayerCharacter> MemberToRemove);

	TArray<TSubclassOf<ARsPlayerCharacter>> GetPartyMemberClasses() const;

private:
	UPROPERTY()
	TArray<TSubclassOf<ARsPlayerCharacter>> PartyMemberClasses;
};
