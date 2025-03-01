// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "RsPlayerController.generated.h"

class URsTargetingComponent;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RS", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URsTargetingComponent> TargetingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGameplayCameraComponent> GameplayCameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UGameplayControlRotationComponent> GameplayControlRotationComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AAIController> PrevAIController;
	
public:
	ARsPlayerController();
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	
	URsPartyComponent* GetPartyComponent() const;
	AAIController* GetPrevAIController() const;
	UGameplayCameraComponent* GetGameplayCameraComponent() const;
};

