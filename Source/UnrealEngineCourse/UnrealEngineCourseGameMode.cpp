// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealEngineCourseGameMode.h"
#include "UnrealEngineCourseCharacter.h"
#include "UnrealEngineCourseTarget.h"
#include "UnrealEngineCourseSaveGame.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

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

void AUnrealEngineCourseGameMode::StartPlay()
{
	Super::StartPlay();
}

namespace
{

	const FString SlotNameString = TEXT("MySlotName");
	const int UserIndexInt32 = 0;

} // namespace

void AUnrealEngineCourseGameMode::SaveGame()
{
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::White, "SaveGame");

	using SaveGame = UUnrealEngineCourseSaveGame;

	SaveGame* SaveGameInstance = Cast<SaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGame::StaticClass()));

	if (SaveGameInstance != nullptr)
	{
		AUnrealEngineCourseCharacter* Character = Cast<AUnrealEngineCourseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		Character->Save(SaveGameInstance);
		SaveGameInstance->Map = UGameplayStatics::GetCurrentLevelName(GetWorld());

		TArray<AActor*> Targets;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUnrealEngineCourseTarget::StaticClass(), Targets);

		for (const AActor* Target : Targets)
		{
			FTargetMemento TargetMemento;

			Target->GetName(TargetMemento.Name);

			TargetMemento.Transform.Location = Target->GetActorLocation();
			TargetMemento.Transform.Rotation = Target->GetActorRotation();
			
			TargetMemento.HitPoints = Cast<AUnrealEngineCourseTarget>(Target)->HitPoints;

			SaveGameInstance->Targets.Add(TargetMemento);
		}

		FAsyncSaveGameToSlotDelegate SavedDelegate;
		SavedDelegate.BindUObject(this, &AUnrealEngineCourseGameMode::OnSaveGameComplete);

		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, SlotNameString, UserIndexInt32, SavedDelegate);
	}
}

void AUnrealEngineCourseGameMode::LoadGame()
{
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::White, "LoadGame");

	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
	LoadedDelegate.BindUObject(this, &AUnrealEngineCourseGameMode::OnLoadGameComplete);

	UGameplayStatics::AsyncLoadGameFromSlot(SlotNameString, UserIndexInt32, LoadedDelegate);
}

void AUnrealEngineCourseGameMode::OnSaveGameComplete(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	if (bSuccess) {
		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::White, "Game Saved Successfully");
	}
}

void AUnrealEngineCourseGameMode::OnLoadGameComplete(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame)
{
	UUnrealEngineCourseSaveGame* SaveGameInstance = Cast<UUnrealEngineCourseSaveGame>(SaveGame);

	if (SaveGameInstance != nullptr)
	{
		//UGameplayStatics::OpenLevel(GetWorld(), FName{ SaveGameInstance->Map });

		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::White, "Game Loaded Successfully");

		AUnrealEngineCourseCharacter* Character = Cast<AUnrealEngineCourseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		Character->Load(SaveGameInstance);
	}

}