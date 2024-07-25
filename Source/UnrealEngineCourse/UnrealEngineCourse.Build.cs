// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealEngineCourse : ModuleRules
{
	public UnrealEngineCourse(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "HTTP", "ImageWrapper", "ImageCore" });
	}
}
