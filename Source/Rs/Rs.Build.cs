// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Rs : ModuleRules
{
	public Rs(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"UMG",
			"GameplayAbilities", 
			"GameplayTasks", 
			"GameplayTags", 
			"ModelViewViewModel",
			"CommonUI",
			"CommonGame",
			"CommonLoadingScreen",
			"ModularGameplayActors",
			"TargetingSystem",
			"GameplayCameras",
			"GameplayMessageRuntime",
			"NavigationSystem",
			"Niagara",
            "DeveloperSettings",
            "AIModule",
            "NetCore",
        });
	}
}
