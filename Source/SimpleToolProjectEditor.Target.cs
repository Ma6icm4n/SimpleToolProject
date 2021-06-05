// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class SimpleToolProjectEditorTarget : TargetRules
{
	public SimpleToolProjectEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "SimpleToolProject" } );
		ExtraModuleNames.AddRange(new string[] { "ToolExampleEditor" });
	}
}
