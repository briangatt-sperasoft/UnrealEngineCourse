// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealEngineCourseSaveGameSystem.h"

#include "UnrealEngineCourseCharacter.h"
#include "UnrealEngineCourseTarget.h"
#include "UnrealEngineCourseSaveGame.h"

#include "Kismet/GameplayStatics.h"

namespace
{

	const FString SlotNameString = TEXT("MySlotName");
	const int UserIndexInt32 = 0;

} // namespace

void UUnrealEngineCourseSaveGameSystem::SaveGame(UUnrealEngineCourseSaveGame* SaveGameInstance)
{
	if (IsValid(SaveGameInstance))
	{
		FAsyncSaveGameToSlotDelegate SavedDelegate;
		SavedDelegate.BindUObject(this, &UUnrealEngineCourseSaveGameSystem::OnSaveGameComplete);

		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, SlotNameString, UserIndexInt32, SavedDelegate);
	}
}

void UUnrealEngineCourseSaveGameSystem::LoadGame()
{
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
	LoadedDelegate.BindUObject(this, &UUnrealEngineCourseSaveGameSystem::OnLoadGameComplete);

	UGameplayStatics::AsyncLoadGameFromSlot(SlotNameString, UserIndexInt32, LoadedDelegate);
}

bool UUnrealEngineCourseSaveGameSystem::DoesSaveGameExist() const
{
	return UGameplayStatics::DoesSaveGameExist(SlotNameString, UserIndexInt32);
}

void UUnrealEngineCourseSaveGameSystem::OnSaveGameComplete(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	if (bSuccess) {
		check(IsValid(GEngine));
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::White, "Game Saved Successfully");
	}
}

void UUnrealEngineCourseSaveGameSystem::OnLoadGameComplete(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame)
{
	UUnrealEngineCourseSaveGame* SaveGameInstance = Cast<UUnrealEngineCourseSaveGame>(SaveGame);

	if (IsValid(SaveGameInstance))
	{
		check(IsValid(GEngine));
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::White, "Game Loaded Successfully");

		// Store as member variable for later access in GameMode::InitGame
		LastSaveState = SaveGameInstance;

		const FString Options = FString::Printf(TEXT("SlotName=%s?UserIndex=%d"), *SlotNameString, UserIndexInt32);
		UGameplayStatics::OpenLevel(GetWorld(), FName{ *SaveGameInstance->Map }, true, Options);
	}
}
