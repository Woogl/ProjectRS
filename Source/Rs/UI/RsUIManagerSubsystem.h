// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameUIManagerSubsystem.h"
#include "RsUIManagerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonDescriptionTextUpdated, UCommonButtonBase*, Button, FText, DescriptionText);

/**
 * 
 */
UCLASS()
class RS_API URsUIManagerSubsystem : public UGameUIManagerSubsystem
{
	GENERATED_BODY()
	
public:
	static URsUIManagerSubsystem* Get(const UObject* WorldContextObject);
	
	UPROPERTY(BlueprintAssignable)
	FOnButtonDescriptionTextUpdated OnButtonDescriptionTextUpdated;
};
