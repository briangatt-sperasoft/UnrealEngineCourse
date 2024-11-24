// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

// Reference: https://dev.epicgames.com/documentation/en-us/unreal-engine/module-properties-in-unreal-engine

public class UnrealEngineCourse : ModuleRules
{
	public UnrealEngineCourse(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		//bAddDefaultIncludePaths = true;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "HTTP", "ImageWrapper", "ImageCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks" });
	}
}
