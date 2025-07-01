// Copyright 2024 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectUIData.h"
#include "RsGameplayEffectUIDataComponent.generated.h"

UCLASS()
class RS_API URsGameplayEffectUIDataComponent : public UGameplayEffectUIData
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "UI", meta=(DisplayThumbnail="true", AllowedClasses="/Script/Engine.Texture,/Script/Engine.MaterialInterface,/Script/Engine.SlateTextureAtlasInterface", DisallowedClasses = "/Script/MediaAssets.MediaTexture"))
	TObjectPtr<UObject> Icon;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	FText Description;
};
