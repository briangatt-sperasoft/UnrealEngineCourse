// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "UnrealEngineCourseSaveGame.h"

#include "UnrealEngineCourseSaveGameSystem.generated.h"

class USaveGame;
class UUnrealEngineCourseSaveGame;

/**
 * 
 */
UCLASS()
class UNREALENGINECOURSE_API UUnrealEngineCourseSaveGameSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SaveGame(UUnrealEngineCourseSaveGame* SaveGameInstance);

	UFUNCTION(BlueprintCallable)
	void LoadGame();

	UFUNCTION(BlueprintPure)
	inline UUnrealEngineCourseSaveGame* GetLatestSaveState() { return LastSaveState; }

private:
	void OnSaveGameComplete(const FString& SlotName, const int32 UserIndex, bool bSuccess);
	void OnLoadGameComplete(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame);

	// NOTE UPROPERTY prevents this instance from being garbage collected (across OpenLevel)
	//		An alternative would be to use a TWeakObjectPtr alongside UObject::AddToRoot, UObject::RemoveFromRoot
	UPROPERTY()
	UUnrealEngineCourseSaveGame* LastSaveState = nullptr;
};
