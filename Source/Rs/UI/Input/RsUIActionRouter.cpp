// Copyright 2025 Team BH.


#include "RsUIActionRouter.h"

void URsUIActionRouter::ApplyUIInputConfig(const FUIInputConfig& NewConfig, bool bForceRefresh)
{
	Super::ApplyUIInputConfig(NewConfig, bForceRefresh);

	// if (APlayerController* PlayerController = GetLocalPlayer()->GetPlayerController(GetWorld()))
	// {
	// 	if (bDesiredCursorVisibility)
	// 	{
	// 		PlayerController->SetShowMouseCursor(true);
	// 	}
	// 	else
	// 	{
	// 		PlayerController->SetShowMouseCursor(false);
	// 		PlayerController->ResetIgnoreLookInput();
	// 		PlayerController->ResetIgnoreMoveInput();
	// 	}
	// }
}
