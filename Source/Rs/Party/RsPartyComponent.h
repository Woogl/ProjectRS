// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RsPartyComponent.generated.h"


class ARsPlayerController;
class ARsPlayerCharacter;

UCLASS()
class RS_API URsPartyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URsPartyComponent();

	ARsPlayerCharacter* GetPartyMember(int32 MemberIndex) const;
	TArray<ARsPlayerCharacter*> GetPartyMembers() const;
	
	void AddPartyMember(ARsPlayerCharacter* NewMember);
	void InsertPartyMember(ARsPlayerCharacter* NewMember, int32 MemberIndex);
	void RemovePartyMember(ARsPlayerCharacter* MemberToRemove);
	
	bool SwitchPartyMember(ARsPlayerController* PlayerController, int32 MemberIndex);
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	TArray<ARsPlayerCharacter*> PartyMembers;
};
