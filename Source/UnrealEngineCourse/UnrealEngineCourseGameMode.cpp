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

	static ConstructorHelpers::FClassFinder<AActor> AmmoPickupClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_AmmoPickup"));
	AmmoPickupClass = AmmoPickupClassFinder.Class;
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
		{
			SaveGameInstance->Map = UGameplayStatics::GetCurrentLevelName(GetWorld());
		}
		
		{
			AUnrealEngineCourseCharacter* Character = Cast<AUnrealEngineCourseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			Character->Save(SaveGameInstance->Player);
		}

		{
			TArray<AActor*> Targets;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUnrealEngineCourseTarget::StaticClass(), Targets);

			for (AActor* Target : Targets)
			{
				FTargetMemento TargetMemento;
				Cast<AUnrealEngineCourseTarget>(Target)->Save(TargetMemento);
				SaveGameInstance->Targets.Add(TargetMemento);
			}
		}

		{
			TArray<AActor*> AmmoPickups;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AmmoPickupClass, AmmoPickups);

			for (AActor* AmmoPickup : AmmoPickups)
			{
				FAmmoPickupMemento PickupMemento;
				AmmoPickup->GetName(PickupMemento.Name);
				SaveGameInstance->AmmoPickups.Add(PickupMemento);
			}
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
	// Restore Target states and remove and Target which has been 'defeated'
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

	// Retain AmmoPickups which have not been picked up in the last game session
	{
		TArray<AActor*> AmmoPickups;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AmmoPickupClass, AmmoPickups);

		for (AActor* AmmoPickup : AmmoPickups)
		{
			// TODO Consider optimizing by sorting the array prior to saving for faster lookups
			const FAmmoPickupMemento* Memento = SaveGame->AmmoPickups.FindByPredicate([AmmoPickup](const FAmmoPickupMemento& SavedAmmoPickup) {
				return SavedAmmoPickup.Name == AmmoPickup->GetName();
			});

			if (Memento == nullptr)
			{
				AmmoPickup->Destroy();
			}
		}
	}
}

void AUnrealEngineCourseGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	AUnrealEngineCourseCharacter* Character = Cast<AUnrealEngineCourseCharacter>(NewPlayer->GetPawn());

	if (Character != nullptr)
	{
		UUnrealEngineCourseSaveGameSystem* System = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UUnrealEngineCourseSaveGameSystem>();

		if ((System != nullptr) && (System->GetLatestSaveState() != nullptr))
		{
			Character->Load(System->GetLatestSaveState()->Player);
		}
	}
}
