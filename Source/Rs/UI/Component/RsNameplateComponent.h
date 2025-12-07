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
	
	UFUNCTION(BlueprintCallable)
	void Initialize(ARsCharacterBase* InOwnerCharacter);
};
