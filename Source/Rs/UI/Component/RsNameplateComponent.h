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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void Initialize(ARsCharacterBase* InOwnerCharacter);

protected:
	void HandleAbilitySystemInitialized();
	
	UFUNCTION()
	void HandlePossessChanged(APawn* OldPawn, APawn* NewPawn);
};
