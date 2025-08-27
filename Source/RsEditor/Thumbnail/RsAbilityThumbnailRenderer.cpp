// Copyright 2025 Team BH.


#include "RsAbilityThumbnailRenderer.h"

#include "CanvasTypes.h"
#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"
#include "Slate/SlateBrushAsset.h"

void URsAbilityThumbnailRenderer::Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* RenderTarget, FCanvas* Canvas, bool bAdditionalViewFamily)
{
    if (const URsGameplayAbility* Ability = Cast<URsGameplayAbility>(Object))
    {
        if (UObject* SkillIcon = Ability->SkillIcon)
        {
            if (const USlateBrushAsset* BrushAsset = MakeBrushAsset(SkillIcon))
            {
                if (const UTexture* TextureAsset = Cast<UTexture>(BrushAsset->Brush.GetResourceObject()))
                {
                    FSlateResourceHandle Handle = BrushAsset->Brush.GetRenderingResource();
                    const FSlateShaderResourceProxy* ResourceProxy = Handle.GetResourceProxy();
                    const FVector2f StartUV = ResourceProxy->StartUV;
                    const FVector2f SizeUV  = ResourceProxy->SizeUV;
                    const FTexture* Texture = TextureAsset->GetResource();
                    Canvas->DrawTile(X, Y, Width, Height, StartUV.X, StartUV.Y, SizeUV.X, SizeUV.Y, FLinearColor::White, Texture);
                    return;
                }
            }
        }
    }
	
	// Fall back to original renderer
	Super::Draw(Object, X, Y, Width, Height, RenderTarget, Canvas, bAdditionalViewFamily);
}

bool URsAbilityThumbnailRenderer::CanVisualizeAsset(UObject* Object)
{
    if (const URsGameplayAbility* Ability = Cast<URsGameplayAbility>(Object))
    {
        return Ability->SkillIcon != nullptr;
    }
    return false;
}

USlateBrushAsset* URsAbilityThumbnailRenderer::MakeBrushAsset(UObject* ResourceObject)
{
    if (!ResourceObject)
    {
        return nullptr;
    }

    if (UTexture* Tex = Cast<UTexture>(ResourceObject))
    {
        USlateBrushAsset* Asset = NewObject<USlateBrushAsset>(this, NAME_None, RF_Transient);
        Asset->Brush = FSlateBrush();
        Asset->Brush.SetResourceObject(Tex);
        Asset->Brush.DrawAs = ESlateBrushDrawType::Image;

        if (UTexture2D* T2D = Cast<UTexture2D>(Tex))
        {
            Asset->Brush.ImageSize = FVector2D(T2D->GetSizeX(), T2D->GetSizeY());
        }
        return Asset;
    }

    if (UMaterialInterface* Material = Cast<UMaterialInterface>(ResourceObject))
    {
        USlateBrushAsset* Asset = NewObject<USlateBrushAsset>(this, NAME_None, RF_Transient);
        Asset->Brush = FSlateBrush();
        Asset->Brush.SetResourceObject(Material);
        Asset->Brush.DrawAs = ESlateBrushDrawType::Image;
        Asset->Brush.ImageSize = FVector2D(128, 128);
        return Asset;
    }
    return nullptr;
}
