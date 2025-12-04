// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PP4 : ModuleRules
{
	public PP4(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "OnlineSubsystem", "OnlineSubsystemUtils", "AIModule", "UMG", "Slate", "SlateCore", "NavigationSystem", "GameplayCameras" });
	}
}
