// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealEngineCourseGameMode.h"
#include "UnrealEngineCourseCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealEngineCourseGameMode::AUnrealEngineCourseGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
