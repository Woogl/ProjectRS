// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CommonPlayerController.h"
#include "Rs/Camera/RsCameraTypes.h"
#include "RsPlayerController.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
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

	UPROPERTY(VisibleAnywhere, Category = "RS|Party")
	TObjectPtr<URsPartyComponent> PartyComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RS|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URsLockOnComponent> LockOnComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RS|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGameplayCameraComponent> GameplayCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> ControllerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ToggleCursorAction;
	
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
	
protected:
	virtual void SetupInputComponent() override;
	
	void HandleToggleCursor(const FInputActionValue& Value);
};

