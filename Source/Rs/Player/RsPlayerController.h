// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CommonPlayerController.h"
#include "GameplayTagContainer.h"
#include "LoadingProcessInterface.h"
#include "Rs/Camera/RsCameraTypes.h"
#include "RsPlayerController.generated.h"

class ARsCameraAnimationActor;
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
	TObjectPtr<UInputAction> ToggleCursorAction;
	
public:
	ARsPlayerController();
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// ILoadingProcessInterface
	virtual bool ShouldShowLoadingScreen(FString& OutReason) const override;

	UPROPERTY()
	TWeakObjectPtr<ARsCameraAnimationActor> CurrentAnimatonCameraActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERsCameraRig CameraRig = ERsCameraRig::FreeCam;

	UFUNCTION(BlueprintCallable)
	URsPartyComponent* GetPartyComponent() const;

	UFUNCTION(BlueprintCallable)
	UGameplayCameraComponent* GetGameplayCameraComponent() const;

	UFUNCTION(BlueprintCallable)
	URsLockOnComponent* GetLockOnComponent() const;

	UFUNCTION(BlueprintPure, Category="RS", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static ARsPlayerController* GetRsPlayerController(const UObject* WorldContextObject);
	
protected:
	virtual void SetupInputComponent() override;
	
	void HandleToggleCursor(const FInputActionValue& Value);
	void HandleOpenMenu(const FInputActionValue& Value, FGameplayTag WidgetTag);
};

