// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UnrealEngineCourseGameMode.generated.h"

class USaveGame;

UCLASS(minimalapi)
class AUnrealEngineCourseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUnrealEngineCourseGameMode();

	UFUNCTION(Exec)
	void SaveGame();

	UFUNCTION(Exec)
	void LoadGame();

	void StartPlay() override;

private:
	void OnSaveGameComplete(const FString& SlotName, const int32 UserIndex, bool bSuccess);
	void OnLoadGameComplete(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame);
};
