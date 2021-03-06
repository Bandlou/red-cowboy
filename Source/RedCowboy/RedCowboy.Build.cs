// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RedCowboy : ModuleRules
{
	public RedCowboy(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine", "InputCore", "AIModule"});

		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}