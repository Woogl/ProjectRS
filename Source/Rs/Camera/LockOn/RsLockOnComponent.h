// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsLockOnComponent.generated.h"


class ARsPlayerCharacter;
class UWidgetComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RS_API URsLockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URsLockOnComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category = "RS|Lock On")
	bool LockOn(AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "RS|Lock On")
	void LockOff();

	UFUNCTION(BlueprintCallable, Category = "RS|Lock On")
	bool TargetingLockOn(const FRsTargetingParams& Params);

	UFUNCTION(BlueprintCallable, Category = "RS|Lock On")
	bool ToggleLockOn(const FRsTargetingParams& Params);

	UFUNCTION(BlueprintPure, Category = "RS|Lock On")
	AActor* GetLockOnTarget() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "RS|Lock On")
	FRsTargetingParams TargetingParams;

	UPROPERTY(EditDefaultsOnly, Category = "RS|Lock On")
	float MaxTargetDistance = 1500.f;

	UPROPERTY(EditDefaultsOnly, Category = "RS|Player")
	float ControlRotationInterpSpeed = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "RS|Player")
	FRotator ControlRotationOffset = FRotator(-20.f, 0.f, 0.f);
	
	UPROPERTY(EditDefaultsOnly, Category = "RS|AI")
	FName BlackBoardValueName = FName("TargetActor");
	
	UPROPERTY(EditDefaultsOnly, Category = "RS|Widget")
	TSubclassOf<UUserWidget> ReticleWidget;

	UPROPERTY(EditDefaultsOnly, Category = "RS|Widget")
	FVector2D ReticleDrawSize = FVector2D(24.f, 24.f);

	UPROPERTY(EditDefaultsOnly, Category = "RS|Widget")
	FName ReticleWidgetSocket = FName("pelvis");

private:
	UWidgetComponent* RespawnReticleWidget(AActor* Target);
	
	UFUNCTION()
	void HandleTargetDeath(AActor* DeadActor);

	UPROPERTY(Transient)
	TObjectPtr<ACharacter> OwnerCharacter;
	
	UPROPERTY(Transient)
	TObjectPtr<AController> OwnerController;
	
	TWeakObjectPtr<AActor> LockOnTarget;
	TWeakObjectPtr<UWidgetComponent> SpawnedReticleWidget;
};
