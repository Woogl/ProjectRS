// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsCharacterBase.h"
#include "Rs/Camera/RsCameraTypes.h"
#include "RsPlayerCharacter.generated.h"

class UGameplayCameraComponent;
class AAIController;
class UInputAction;
struct FInputActionValue;
class UInputMappingContext;
/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class RS_API ARsPlayerCharacter : public ARsCharacterBase
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RS|Character", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> JustDodgeCapsule;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RS|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGameplayCameraComponent> GameplayCameraComponent;
	
public:
	ARsPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	// Called on the server to acknowledge possession of this Character.
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	
	virtual void OnRep_PlayerState() override;
	virtual void OnRep_Controller() override;

	void SetupAbilityInput();
	
	UFUNCTION(Client, Unreliable)
	void SetupCamera_Client();
	
	ERsCameraRig CameraRig = ERsCameraRig::ThirdPersonView;

private:
	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);

public:
	// Reference of the previous possessed friendly AI controller.
	UPROPERTY(Transient)
	TObjectPtr<AAIController> FriendlyAIController = nullptr;
	
	UFUNCTION(BlueprintPure)
	ERsCameraRig GetCameraRig() const;
	
	UFUNCTION(BlueprintCallable)
	void SetCameraRig(ERsCameraRig InCameraRig);
	
	UFUNCTION(BlueprintCallable)
	void EnableJustDodgeCapsule(bool bEnable = true);

	UGameplayCameraComponent* GetGameplayCameraComponent() const;
	UInputMappingContext* GetDefaultMappingContext() const;
};
