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

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AController> PrevController;
	
public:
	ARsPlayerController();
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	TObjectPtr<AController> GetPrevController() const;
	URsPartyComponent* GetPartyComponent() const;
};

