// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CommonPlayerController.h"
#include "GameplayTagContainer.h"
#include "LoadingProcessInterface.h"
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
class RS_API ARsPlayerController : public ACommonPlayerController, public IAbilitySystemInterface, public ILoadingProcessInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "RS|Party", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URsPartyComponent> PartyComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RS|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URsLockOnComponent> LockOnComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RS|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGameplayCameraComponent> GameplayCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> ControllerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Input", meta = (AllowPrivateAccess = "true", Categories = "UI.Menu"))
	TMap<FGameplayTag, UInputAction*> OpenMenuActions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ShowCursorAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ResetCursorAction;
	
public:
	ARsPlayerController();
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// ILoadingProcessInterface
	virtual bool ShouldShowLoadingScreen(FString& OutReason) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERsCameraRig CameraRig = ERsCameraRig::FreeCam;
	
	URsPartyComponent* GetPartyComponent() const;
	UGameplayCameraComponent* GetGameplayCameraComponent() const;
	URsLockOnComponent* GetLockOnComponent() const;
	
protected:
	virtual void SetupInputComponent() override;
	
	void HandleShowCursor(const FInputActionValue& Value);
	void HandleResetCursor(const FInputActionValue& Value);
	void HandleOpenMenu(const FInputActionValue& Value, FGameplayTag WidgetTag);
};

