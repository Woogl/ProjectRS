// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CommonPlayerController.h"
#include "Rs/Camera/RsCameraTypes.h"
#include "RsPlayerController.generated.h"

class URsLockOnComponent;
class AAIController;
class UGameplayCameraComponent;
class URsPartyComponent;

/**
 * 
 */
UCLASS()
class RS_API ARsPlayerController : public ACommonPlayerController, public IAbilitySystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Party")
	TObjectPtr<URsPartyComponent> PartyComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URsLockOnComponent> LockOnComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGameplayCameraComponent> GameplayCameraComponent;
	
public:
	ARsPlayerController();
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ERsCameraRig CameraRig = ERsCameraRig::ThirdPerson;
	
	URsPartyComponent* GetPartyComponent() const;
	UGameplayCameraComponent* GetGameplayCameraComponent() const;
	URsLockOnComponent* GetLockOnComponent() const;

	UFUNCTION(BlueprintPure, Category="RS", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static APlayerController* GetRsPlayerController(const UObject* WorldContextObject);
};

