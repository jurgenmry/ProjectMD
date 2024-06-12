// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectMeltdown : ModuleRules
{
	public ProjectMeltdown(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayAbilities", "GameplayTags", "GameplayTasks","UMG", "Slate", "SlateCore", "NetCore", "Niagara" });
        PrivateDependencyModuleNames.AddRange(new string[] {  });
    }
}
