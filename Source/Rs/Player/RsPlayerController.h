// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CommonPlayerController.h"
#include "RsPlayerController.generated.h"

class URsLockOnComponent;
class AAIController;
class UGameplayControlRotationComponent;
class UGameplayCameraComponent;
class ARsPlayerCharacter;
class URsPartyComponent;

UENUM(BlueprintType)
enum class ERsCameraMode : uint8
{
	ThirdPerson,
	ShoulderView,
	LockOn,
	LinkSkill,
};

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

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AAIController> PrevAIController;
	
public:
	ARsPlayerController();
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ERsCameraMode CameraMode = ERsCameraMode::ThirdPerson;

	AAIController* GetPrevAIController() const;
	
	URsPartyComponent* GetPartyComponent() const;
	UGameplayCameraComponent* GetGameplayCameraComponent() const;
	URsLockOnComponent* GetLockOnComponent() const;
};

