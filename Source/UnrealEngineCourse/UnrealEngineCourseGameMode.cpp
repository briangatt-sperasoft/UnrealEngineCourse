// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealEngineCourseGameMode.h"
#include "UnrealEngineCourseCharacter.h"
#include "UObject/ConstructorHelpers.h"

#include <GameFramework/HUD.h>

AUnrealEngineCourseGameMode::AUnrealEngineCourseGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// set default HUD class to our Blueprinted HUD
	static ConstructorHelpers::FClassFinder<AHUD> HUDClassFinder(TEXT("/Game/FirstPerson/Blueprints/HUD/BP_HUD"));
	HUDClass = HUDClassFinder.Class;

}
