#include "RsEditor.h"

#include "Rs/AbilitySystem/Abilities/RsGameplayAbility.h"
#include "Thumbnail/RsAbilityThumbnailRenderer.h"
#include "ThumbnailRendering/ThumbnailManager.h"

#define LOCTEXT_NAMESPACE "FRsEditorModule"

void FRsEditorModule::StartupModule()
{
	UThumbnailManager::Get().RegisterCustomRenderer(URsGameplayAbility::StaticClass(), URsAbilityThumbnailRenderer::StaticClass());
}

void FRsEditorModule::ShutdownModule()
{
	UThumbnailManager::Get().UnregisterCustomRenderer(URsGameplayAbility::StaticClass());
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FRsEditorModule, RsEditor)