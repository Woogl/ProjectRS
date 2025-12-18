// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "RsNameplateComponent.generated.h"

class ARsCharacterBase;
/**
 * 
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class RS_API URsNameplateComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	URsNameplateComponent();
	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void Initialize(ARsCharacterBase* InOwnerCharacter);
	
protected:
	void UpdateVisibility();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShowDistance = 1000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HideDistance = 1200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceCheckInterval = 0.5f;
	
	FTimerHandle DistanceCheckTimer;
};
