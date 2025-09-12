// Copyright 2025 Team BH.


#include "RsUIActionRouter.h"

#include "CommonUI/Private/Input/UIActionRouterTypes.h"


void URsUIActionRouter::ApplyLeafmostNodeConfig()
{
	if (FActivatableTreeRootPtr ActiveRoot = GetActiveRoot().Pin())
	{
		ActiveRoot->ApplyLeafmostNodeConfig();
	}
}
