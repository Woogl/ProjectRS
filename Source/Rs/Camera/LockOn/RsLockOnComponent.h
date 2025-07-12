// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Rs/Targeting/RsTargetingTypes.h"
#include "RsLockOnComponent.generated.h"


class UWidgetComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RS_API URsLockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URsLockOnComponent();
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);

	UFUNCTION(BlueprintCallable, Category = "RS|Lock On")
	bool ToggleLockOn();
	
	UFUNCTION(BlueprintCallable, Category = "RS|Lock On")
	bool LockOn(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "RS|Lock On")
	void LockOff();

	UFUNCTION(BlueprintPure, Category = "RS|Lock On")
	AActor* GetLockOnTarget() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "RS")
	FGameplayTag LockOnAbilityTag;

	UPROPERTY(EditDefaultsOnly, Category = "RS")
	FRsTargetingCollision TargetingCollision;

	UPROPERTY(EditDefaultsOnly, Category = "RS")
	FRsTargetingFilter TargetingFilter;

	UPROPERTY(EditDefaultsOnly, Category = "RS")
	FRsTargetingSorter TargetingSorter;

	UPROPERTY(EditDefaultsOnly, Category = "RS")
	float MaxTargetDistance = 5000.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> ReticleWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	FVector2D ReticleDrawSize = FVector2D(24.f, 24.f);

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	FName ReticleWidgetSocket = FName("pelvis");

	UFUNCTION()
	void HandleDeathStarted(AActor* DeadActor);

private:
	TWeakObjectPtr<AActor> LockOnTarget;
	
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TWeakObjectPtr<UWidgetComponent> ReticleComponent;
};
