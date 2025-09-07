// Copyright 2025 Team BH.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "LoadingProcessInterface.h"
#include "GameFramework/HUD.h"
#include "RsHUD.generated.h"

enum class ERsLinkSkillType : uint8;
class ARsCharacterBase;
class URsHUDLayout;
class URsActivatableWidget;
/**
 * 
 */
UCLASS()
class RS_API ARsHUD : public AHUD, public ILoadingProcessInterface
{
	GENERATED_BODY()

public:
	ARsHUD();

	// AHUD interface
	virtual void GetDebugActorList(TArray<AActor*>& InOutList) override;

	// ILoadingProcessInterface
	virtual bool ShouldShowLoadingScreen(FString& OutReason) const override;

	// Returns the LocalPlayer for this HUD's player.
	UFUNCTION(BlueprintCallable, Category = "HUD")
	ULocalPlayer* GetOwningLocalPlayer() const;
	
	void OpenGameHUD();
	void OpenMenuWidget(FGameplayTag WidgetTag, bool bSuspendInputUntilComplete = false);
	// TODO: void OpenLoadingScreen();
	
	URsHUDLayout* GetGameHUD() const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void HandleLinkSkillReady(ARsCharacterBase* Target, ERsLinkSkillType Type, int32 AvailableCount);
	void HandleDeactivateMenuWidget();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSoftClassPtr<URsHUDLayout> GameHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (Categories = "UI.Menu", ForceInlineRow))
	TMap<FGameplayTag, TSoftClassPtr<URsActivatableWidget>> MenuWidgets;

private:
	UPROPERTY(Transient)
	TObjectPtr<URsHUDLayout> GameHUDInstance;
};
