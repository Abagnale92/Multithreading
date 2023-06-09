// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Multithread : ModuleRules
{
	public Multithread(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "ProceduralMeshComponent", "MeshDescription", "UMG" });
    }
}