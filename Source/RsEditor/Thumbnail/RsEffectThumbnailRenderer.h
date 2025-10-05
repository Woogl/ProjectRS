// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "ThumbnailRendering/TextureThumbnailRenderer.h"
#include "RsEffectThumbnailRenderer.generated.h"

/**
 * 
 */
UCLASS()
class RSEDITOR_API URsEffectThumbnailRenderer : public UTextureThumbnailRenderer
{
	GENERATED_BODY()
	
public:
	virtual void Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget*, FCanvas* Canvas, bool bAdditionalViewFamily) override;
	virtual bool CanVisualizeAsset(UObject* Object) override;
};
