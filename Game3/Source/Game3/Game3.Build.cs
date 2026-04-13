// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Game3 : ModuleRules
{
	public Game3(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		bEnableExceptions = true;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "ZeroMQ" });
	}
}
