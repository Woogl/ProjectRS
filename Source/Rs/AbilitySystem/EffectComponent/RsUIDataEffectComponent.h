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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "UI", meta=(DisplayThumbnail="true", AllowedClasses="MaterialInterface,Texture2D"))
	TObjectPtr<UObject> Icon;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FText Description;
};
