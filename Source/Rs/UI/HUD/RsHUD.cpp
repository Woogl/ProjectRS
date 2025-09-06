// Copyright 2025 Team BH.


#include "RsHUD.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "PrimaryGameLayout.h"
#include "Rs/RsGameplayTags.h"
#include "RsHUDLayout.h"
#include "Rs/Battle/Subsystem/RsBattleSubsystem.h"
#include "Rs/System/RsGameSettingDataAsset.h"

ARsHUD::ARsHUD()
{
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ARsHUD::GetDebugActorList(TArray<AActor*>& InOutList)
{
	UWorld* World = GetWorld();
	
	Super::GetDebugActorList(InOutList);

	// Add all actors with an ability system component.
	for (TObjectIterator<UAbilitySystemComponent> It; It; ++It)
	{
		if (UAbilitySystemComponent* ASC = *It)
		{
			if (!ASC->HasAnyFlags(RF_ClassDefaultObject | RF_ArchetypeObject))
			{
				AActor* AvatarActor = ASC->GetAvatarActor();
				AActor* OwnerActor = ASC->GetOwnerActor();
				
				if (AvatarActor && UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(AvatarActor))
				{
					AddActorToDebugList(AvatarActor, InOutList, World);
				}
				else if (OwnerActor && UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerActor))
				{
					AddActorToDebugList(OwnerActor, InOutList, World);
				}
			}
		}
	}
}

ULocalPlayer* ARsHUD::GetOwningLocalPlayer() const
{
	if (APlayerController* PlayerController = GetOwningPlayerController())
	{
		return PlayerController->GetLocalPlayer();
	}
	return nullptr;
}

void ARsHUD::OpenGameHUD()
{
	UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayout(GetOwningPlayerController());
	if (GameHUD.IsNull() || !RootLayout)
	{
		return;
	}
	
	RootLayout->PushWidgetToLayerStackAsync<URsActivatableWidget>(RsGameplayTags::UI_LAYER_MENU, false, GameHUD, [this](EAsyncWidgetLayerState State, URsActivatableWidget* Widget)
	{
		if (!Widget)
		{
			return;
		}
		switch (State)
		{
		case EAsyncWidgetLayerState::Initialize:
			break;
		case EAsyncWidgetLayerState::AfterPush:
			GameHUDInstance = Cast<URsHUDLayout>(Widget);
			break;
		case EAsyncWidgetLayerState::Canceled:
			break;
		}
	});
}

void ARsHUD::OpenMenuWidget(FGameplayTag WidgetTag, bool bSuspendInputUntilComplete)
{
	TSoftClassPtr<URsActivatableWidget> MenuWidget = MenuWidgets.FindRef(WidgetTag);
	UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayout(GetOwningPlayerController());
	if (MenuWidget.IsNull() || !RootLayout)
	{
		return;
	}

	RootLayout->PushWidgetToLayerStackAsync<URsActivatableWidget>(RsGameplayTags::UI_LAYER_MENU, bSuspendInputUntilComplete, MenuWidget, [this](EAsyncWidgetLayerState State, URsActivatableWidget* Widget)
	{
		if (!Widget)
		{
			return;
		}
		switch (State)
		{
		case EAsyncWidgetLayerState::Initialize:
			break;
		case EAsyncWidgetLayerState::AfterPush:
			if (GameHUDInstance)
			{
				GameHUDInstance->SetVisibility(ESlateVisibility::Hidden);
			}
			Widget->OnDeactivated().AddUObject(this, &ThisClass::HandleDeactivateMenuWidget);
			break;
		case EAsyncWidgetLayerState::Canceled:
			break;
		}
	});
}

URsHUDLayout* ARsHUD::GetGameHUD() const
{
	return GameHUDInstance;
}

void ARsHUD::BeginPlay()
{
	Super::BeginPlay();

	OpenGameHUD();

	if (URsBattleSubsystem* BattleSubsystem = GetOwningLocalPlayer()->GetSubsystem<URsBattleSubsystem>())
	{
		BattleSubsystem->OnLinkSkillReady.AddUObject(this, &ThisClass::HandleLinkSkillReady);
	}
}

void ARsHUD::HandleLinkSkillReady(ARsCharacterBase* Target, ERsLinkSkillType Type, int32 AvailableCount)
{
	if (Type == ERsLinkSkillType::Triple)
	{
		FGameplayTag TripleLinkSkillTag = URsGameSettingDataAsset::Get()->TripleLinkSkillTag;
		OpenMenuWidget(TripleLinkSkillTag, false);
	}
}

void ARsHUD::HandleDeactivateMenuWidget()
{
	if (GameHUDInstance)
	{
		GameHUDInstance->SetVisibility(ESlateVisibility::Visible);
	}
}
