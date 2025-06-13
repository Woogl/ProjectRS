// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsCharacterBase.h"
#include "RsPlayerCharacter.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RS", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> PerfectDodgeCapsuleComponent;
	
public:
	ARsPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	// Called on the server to acknowledge possession of this Character.
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	// Called on the client when the Character is assigned its Player State.
	virtual void OnRep_PlayerState() override;

	void InitAbilitySystem();

private:
	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);

public:
	// Reference of the previous possessed friendly AI controller.
	UPROPERTY()
	TObjectPtr<AAIController> FriendlyAIController = nullptr;
	
	FORCEINLINE UCapsuleComponent* GetPerfectDodgeCapsuleComponent() const { return PerfectDodgeCapsuleComponent; }
	FORCEINLINE UInputMappingContext* GetDefaultMappingContext() const { return DefaultMappingContext; }
};
