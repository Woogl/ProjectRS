// Copyright 2024 Team BH.


#include "RsNameplateComponent.h"

#include "Extensions/UserWidgetExtension.h"
#include "Kismet/GameplayStatics.h"
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

	if (GetWidget())
	{
		if (ARsCharacterBase* OwnerCharacter = Cast<ARsCharacterBase>(GetOwner()))
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
					PlayerController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::HandlePossessChanged);
				}
			}
		}
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
