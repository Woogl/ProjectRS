// Copyright 2024 Team BH.


#include "RsNameplateComponent.h"

#include "Extensions/UserWidgetExtension.h"
#include "Kismet/GameplayStatics.h"
#include "Rs/AbilitySystem/RsAbilitySystemComponent.h"
#include "Rs/Character/RsPlayerCharacter.h"
#include "Rs/UI/MVVM/ViewModel/RsCharacterViewModel.h"
#include "View/MVVMView.h"

URsNameplateComponent::URsNameplateComponent()
{
	Space = EWidgetSpace::Screen;
	bDrawAtDesiredSize = true;
}

void URsNameplateComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (URsAbilitySystemComponent* RsASC = URsAbilitySystemComponent::GetAbilitySystemComponentFromActor(GetOwner()))
	{
		RsASC->CallOrRegister_OnAbilitySystemInitialized(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::HandleAbilitySystemInitialized));
	}
}

void URsNameplateComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetOwner() && GetOwner()->IsA(ARsPlayerCharacter::StaticClass()))
	{
		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			PlayerController->OnPossessedPawnChanged.RemoveDynamic(this, &ThisClass::HandlePossessChanged);
		}
	}
	
	Super::EndPlay(EndPlayReason);
}

void URsNameplateComponent::Initialize(ARsCharacterBase* InOwnerCharacter)
{
	if (!GetWidget())
	{
		return;
	}
	
	if (ARsCharacterBase* OwnerCharacter = Cast<ARsCharacterBase>(InOwnerCharacter))
	{
		if (URsCharacterViewModel* CharacterViewModel = URsCharacterViewModel::CreateRsCharacterViewModel(OwnerCharacter))
		{
			if (UMVVMView* MVVM = Cast<UMVVMView>(GetWidget()->GetExtension(UMVVMView::StaticClass())))
			{
				MVVM->SetViewModelByClass(CharacterViewModel);
			}
		}

		if (OwnerCharacter->IsA(ARsPlayerCharacter::StaticClass()))
		{
			if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				HandlePossessChanged(nullptr, nullptr);
				PlayerController->OnPossessedPawnChanged.AddUniqueDynamic(this, &ThisClass::HandlePossessChanged);
			}
		}
	}
}

void URsNameplateComponent::HandleAbilitySystemInitialized()
{
	if (ARsCharacterBase* OwnerCharacter = Cast<ARsCharacterBase>(GetOwner()))
	{
		Initialize(OwnerCharacter);
	}
}

void URsNameplateComponent::HandlePossessChanged(APawn* OldPawn, APawn* NewPawn)
{
	if (GetOwner() == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		SetVisibility(false);
	}
	else
	{
		SetVisibility(true);
	}
}
