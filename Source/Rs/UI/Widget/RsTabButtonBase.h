// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "RsButtonBase.h"
#include "RsTabListWidgetBase.h"
#include "RsTabButtonBase.generated.h"

struct FRsTabDescriptor;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, meta = (DisableNativeTick))
class RS_API URsTabButtonBase : public URsButtonBase, public IRsTabButtonInterface
{
	GENERATED_BODY()
	
public:
	void SetIconFromLazyObject(TSoftObjectPtr<UObject> LazyObject);
	void SetIconBrush(const FSlateBrush& Brush);

protected:

	UFUNCTION()
	virtual void SetTabLabelInfo_Implementation(const FRsTabDescriptor& TabLabelInfo) override;

private:

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonLazyImage> LazyImage_Icon;
};
