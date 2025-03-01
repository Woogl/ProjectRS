// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RsTargetingComponent.generated.h"


class UWidgetComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RS_API URsTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URsTargetingComponent();

	UFUNCTION(BlueprintCallable, Category = "RS|Targeting")
	void LockOn(AActor* InTarget);

	UFUNCTION(BlueprintCallable, Category = "RS|Targeting")
	void LockOff();

	UFUNCTION(BlueprintPure, Category = "RS|Targeting")
	bool HasLockOnTarget() const;

	UFUNCTION(BlueprintPure, Category = "RS|Targeting")
	AActor* GetLockedOnTarget() const;

	UFUNCTION(BlueprintPure, Category = "RS|Targeting", Meta=(DefaultToSelf = "Actor"))
	static URsTargetingComponent* GetTargetingComponent(AActor* Actor);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> ReticleWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	FVector2D ReticleDrawSize = FVector2D(24.f, 24.f);

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	FName ReticleWidgetSocket = FName("pelvis");
	
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TWeakObjectPtr<AActor> LockedOnTarget;
	
	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TObjectPtr<UWidgetComponent> ReticleComponent;
};
