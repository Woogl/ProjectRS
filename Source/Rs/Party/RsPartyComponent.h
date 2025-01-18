// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RsPartyComponent.generated.h"


class ARsPlayerCharacter;

UCLASS()
class RS_API URsPartyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URsPartyComponent();

	ARsPlayerCharacter* GetPartyMember(int32 MemberIndex) const;
	void AddPartyMember(ARsPlayerCharacter* NewMember);
	void RemovePartyMember(ARsPlayerCharacter* MemberToRemove);
	
protected:
	UPROPERTY(VisibleAnywhere)
	TArray<ARsPlayerCharacter*> PartyMembers;
};
