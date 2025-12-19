// Copyright 2025 Team BH.


#include "RsEffectThumbnailRenderer.h"

#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "Rs/AbilitySystem/Effect/RsGameplayEffect.h"
#include "Rs/AbilitySystem/EffectComponent/RsUIDataEffectComponent.h"

void URsEffectThumbnailRenderer::Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* RenderTarget, FCanvas* Canvas, bool bAdditionalViewFamily)
{
	if (const UGameplayEffect* Effect = Cast<UGameplayEffect>(Object))
	{
		if (const URsUIDataEffectComponent* UIData = Effect->FindComponent<URsUIDataEffectComponent>())
		{
			if (const UObject* IconObject = UIData->GetIcon())
			{
				// Texture
				if (const UTexture2D* Texture = Cast<UTexture2D>(IconObject))
				{
					if (const FTexture* Resource = Texture->GetResource())
					{
						Canvas->DrawTile(X, Y, Width, Height, 0.f, 0.f, 1.f, 1.f, FLinearColor::White, Resource);
						return;
					}
				}

				// Material
				if (const UMaterialInterface* Material = Cast<UMaterialInterface>(IconObject))
				{
					FCanvasTileItem Tile(FVector2D(X, Y), FVector2D(Width, Height), FLinearColor::White);
					Tile.BlendMode = SE_BLEND_Translucent;
					Tile.MaterialRenderProxy = Material->GetRenderProxy();
					Canvas->DrawItem(Tile);
					return;
				}
			}
		}
	}
    
	Super::Draw(Object, X, Y, Width, Height, RenderTarget, Canvas, bAdditionalViewFamily);
}

bool URsEffectThumbnailRenderer::CanVisualizeAsset(UObject* Object)
{
	if (const UGameplayEffect* Effect = Cast<UGameplayEffect>(Object))
	{
		return Effect->FindComponent<URsUIDataEffectComponent>() != nullptr;
	}
	return false;
}