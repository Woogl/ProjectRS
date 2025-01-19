// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "RsPlayerController.generated.h"

class ARsPlayerCharacter;
class URsPartyComponent;
/**
 * 
 */
UCLASS()
class RS_API ARsPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Party")
	TObjectPtr<URsPartyComponent> PartyComponent;
	
public:
	ARsPlayerController();

	URsPartyComponent* GetPartyComponent() const;
};
