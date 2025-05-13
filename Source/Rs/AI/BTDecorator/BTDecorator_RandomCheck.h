// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_RandomCheck.generated.h"

/**
 * 
 */
UCLASS()
class RS_API UBTDecorator_RandomCheck : public UBTDecorator
{
	GENERATED_BODY()

public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Probability (%)", ClampMin="0.0", ClampMax="100.0", UIMin="0.0", UIMax="100.0", SliderExponent=1.0))
	float Probability;
};
