// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class NickAlexNate_MenuSysEditorTarget : TargetRules
{
	public NickAlexNate_MenuSysEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "NickAlexNate_MenuSys" } );
	}
}
