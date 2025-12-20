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
	
	UpdateVisibility();

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(DistanceCheckTimer, this, &ThisClass::UpdateVisibility, DistanceCheckInterval, true);
	}
}

void URsNameplateComponent::Initialize(ARsCharacterBase* InOwnerCharacter)
{
	if (!GetWidget() || !InOwnerCharacter)
	{
		return;
	}
	
	if (URsCharacterViewModel* CharacterViewModel = URsCharacterViewModel::CreateRsCharacterViewModel(InOwnerCharacter))
	{
		if (UMVVMView* MVVM = Cast<UMVVMView>(GetWidget()->GetExtension(UMVVMView::StaticClass())))
		{
			MVVM->SetViewModelByClass(CharacterViewModel);
			SetVisibility(true);
		}
	}
}

void URsNameplateComponent::UpdateVisibility()
{
	const AActor* Owner = GetOwner();
	const APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	UUserWidget* MyWidget = GetWidget();
	if (!Owner || !PlayerPawn || !MyWidget)
	{
		return;
	}
	
	FVector OwnerLocation = Owner->GetActorLocation();
	FVector PlayerLocation = PlayerPawn->GetActorLocation();
	float DistSquared = FVector::DistSquared(OwnerLocation, PlayerLocation);
	
	if (DistSquared > (HideDistance * HideDistance))
	{
		MyWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	else if (DistSquared < (ShowDistance * ShowDistance))
	{
		MyWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}
