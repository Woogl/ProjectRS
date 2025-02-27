// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "RsPlayerController.generated.h"

class AAIController;
class UGameplayControlRotationComponent;
class UGameplayCameraComponent;
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
	TObjectPtr<AAIController> PrevAIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGameplayCameraComponent> GameplayCameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UGameplayControlRotationComponent> GameplayControlRotationComponent;
	
public:
	ARsPlayerController();
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	
	URsPartyComponent* GetPartyComponent() const;
	AAIController* GetPrevAIController() const;
	UGameplayCameraComponent* GetGameplayCameraComponent() const;
};

