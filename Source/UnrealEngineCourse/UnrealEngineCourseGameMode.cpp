// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealEngineCourseGameMode.h"

#include "UnrealEngineCourseSaveGameSystem.h"
#include "UnrealEngineCourseCharacter.h"
#include "UnrealEngineCourseTarget.h"

#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/PlayerStart.h"
#include "GameFramework/HUD.h"

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

void AUnrealEngineCourseGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// TODO Potentially load the game using the specified details
	FString SlotName = UGameplayStatics::ParseOption(Options, "SlotName");
	int32 UserIndex = UGameplayStatics::GetIntOption(Options, "UserIndex", 0);

	if (!SlotName.IsEmpty())
	{
		UUnrealEngineCourseSaveGameSystem* System = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UUnrealEngineCourseSaveGameSystem>();
		
		if ((System != nullptr) && (System->GetLatestSaveState() != nullptr))
		{
			LoadGame(System->GetLatestSaveState());
		}
	}
}

void AUnrealEngineCourseGameMode::SaveGame()
{
	using SaveGame = UUnrealEngineCourseSaveGame;

	SaveGame* SaveGameInstance = Cast<SaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGame::StaticClass()));

	if (SaveGameInstance != nullptr)
	{
		AUnrealEngineCourseCharacter* Character = Cast<AUnrealEngineCourseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		Character->Save(SaveGameInstance->Player);
		SaveGameInstance->Map = UGameplayStatics::GetCurrentLevelName(GetWorld());

		TArray<AActor*> Targets;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUnrealEngineCourseTarget::StaticClass(), Targets);

		for (AActor* Target : Targets)
		{
			FTargetMemento TargetMemento;

			Cast<AUnrealEngineCourseTarget>(Target)->Save(TargetMemento);

			SaveGameInstance->Targets.Add(TargetMemento);
		}
	}

	UUnrealEngineCourseSaveGameSystem* System = GetGameInstance()->GetSubsystem<UUnrealEngineCourseSaveGameSystem>();
	System->SaveGame(SaveGameInstance);
}

void AUnrealEngineCourseGameMode::LoadGame()
{
	UUnrealEngineCourseSaveGameSystem* System = GetGameInstance()->GetSubsystem<UUnrealEngineCourseSaveGameSystem>();
	System->LoadGame();
}

void AUnrealEngineCourseGameMode::LoadGame(const UUnrealEngineCourseSaveGame* SaveGame)
{
	{
		TArray<AActor*> PlayerStart;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStart);

		PlayerStart[0]->SetActorTransform(SaveGame->Player.Transform);
	}

	{
		TArray<AActor*> Targets;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUnrealEngineCourseTarget::StaticClass(), Targets);

		for (AActor* Target : Targets)
		{
			const FTargetMemento* Memento = SaveGame->Targets.FindByPredicate([Target](const FTargetMemento& SavedTarget) {
				return SavedTarget.Name == Target->GetName();
			});

			if (Memento != nullptr)
			{
				Cast<AUnrealEngineCourseTarget>(Target)->Load(*Memento);
			}
			else
			{
				Target->Destroy();
			}
		}
	}
}
