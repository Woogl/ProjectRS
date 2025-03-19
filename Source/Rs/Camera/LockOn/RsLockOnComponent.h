// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RsLockOnComponent.generated.h"


class UWidgetComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RS_API URsLockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URsLockOnComponent();

	UFUNCTION(BlueprintCallable, Category = "RS|Lock On")
	void LockOn(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "RS|Lock On")
	void LockOff();

	UFUNCTION(BlueprintPure, Category = "RS|Lock On")
	bool HasLockOnTarget() const;

	UFUNCTION(BlueprintPure, Category = "RS|Lock On")
	AActor* GetLockedOnTarget() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> ReticleWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	FVector2D ReticleDrawSize = FVector2D(24.f, 24.f);

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	FName ReticleWidgetSocket = FName("pelvis");

	UFUNCTION()
	void HandleDeathStarted(AActor* DeadActor);

private:
	TWeakObjectPtr<AActor> LockedOnTarget;
	
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TWeakObjectPtr<UWidgetComponent> ReticleComponent;
};
