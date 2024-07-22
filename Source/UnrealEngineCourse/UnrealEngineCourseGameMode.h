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

	UPROPERTY()
	TSubclassOf<AActor> AmmoPickupClass;

	UPROPERTY()
	TSubclassOf<AActor> WeaponPickupClass;

public:
	AUnrealEngineCourseGameMode();

	// AGameModeBase overrides
	void StartPlay() override;
	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	// AGameModeBase overrides
	
	UFUNCTION(Exec, BlueprintCallable)
	void SaveGame();

	UFUNCTION(Exec, BlueprintCallable)
	void LoadGame();

	UFUNCTION(BlueprintPure)
	bool DoesSaveGameExist() const;

	// NOTE The '_Implementation' suffix is standard for BlueprintNativeEvent
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

private:
	void LoadGame(const UUnrealEngineCourseSaveGame* SaveGame);
};
