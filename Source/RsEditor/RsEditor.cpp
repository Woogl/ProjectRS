#include "RsEditor.h"

#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"
#include "Rs/AbilitySystem/Effect/RsGameplayEffect.h"
#include "Thumbnail/RsAbilityThumbnailRenderer.h"
#include "Thumbnail/RsEffectThumbnailRenderer.h"
#include "ThumbnailRendering/ThumbnailManager.h"

#define LOCTEXT_NAMESPACE "FRsEditorModule"

void FRsEditorModule::StartupModule()
{
	UThumbnailManager::Get().RegisterCustomRenderer(URsGameplayAbility::StaticClass(), URsAbilityThumbnailRenderer::StaticClass());
	UThumbnailManager::Get().RegisterCustomRenderer(UGameplayEffect::StaticClass(), URsEffectThumbnailRenderer::StaticClass());
}

void FRsEditorModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FRsEditorModule, RsEditor)