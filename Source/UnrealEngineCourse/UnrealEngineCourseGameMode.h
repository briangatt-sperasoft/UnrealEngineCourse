// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UnrealEngineCourseGameMode.generated.h"

class UUnrealEngineCourseSaveGame;

UCLASS(minimalapi)
class AUnrealEngineCourseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUnrealEngineCourseGameMode();

	// AGameModeBase overrides
	void StartPlay() override;
	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	// AGameModeBase overrides
	
	UFUNCTION(Exec)
	void SaveGame();

	UFUNCTION(Exec)
	void LoadGame();

private:
	void LoadGame(const UUnrealEngineCourseSaveGame* SaveGame);
};
