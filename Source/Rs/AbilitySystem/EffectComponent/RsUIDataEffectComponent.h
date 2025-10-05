// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectUIData.h"
#include "RsUIDataEffectComponent.generated.h"

UCLASS()
class RS_API URsUIDataEffectComponent : public UGameplayEffectUIData
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "RS | UI")
	UObject* GetIcon() const { return Icon; }

	UFUNCTION(BlueprintCallable, Category = "RS | UI")
	FText GetDescription() const { return Description; }

	UFUNCTION(BlueprintCallable, Category = "RS | UI")
	int32 GetPriority() const { return Priority; }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "RS | UI", meta=(DisplayThumbnail="true", AllowedClasses="MaterialInterface,Texture2D", AllowPrivateAccess = true))
	TObjectPtr<UObject> Icon;

	UPROPERTY(EditDefaultsOnly, Category = "RS | UI", meta = (AllowPrivateAccess = true))
	FText Description;

	UPROPERTY(EditDefaultsOnly, Category = "RS | UI", meta = (AllowPrivateAccess = true))
	int32 Priority = 0;
};
