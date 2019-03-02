// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class ARPG : ModuleRules
{
	public ARPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[]
		{
            "AIModule",
            "NavigationSystem",
            "GameplayTasks",
            "GameplayTags",


            "XD_UnrealLibraryEx",
			"XD_SaveGameSystem",
			"XD_TimeSystem",
			"XD_MovementComponentPlugin",
			"XD_GenericGraph",
			"XD_EnvironmentSystem",
			"XD_CampSystem",
            "XD_ItemSystem",
            "XD_AI_SystemEx",
            "XD_EventFlowSystem",
            "XD_CharacterActionDispatcher",
        });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
