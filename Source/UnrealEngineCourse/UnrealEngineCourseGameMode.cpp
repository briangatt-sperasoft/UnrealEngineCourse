// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealEngineCourseGameMode.h"

#include "UnrealEngineCourseSaveGameSystem.h"
#include "UnrealEngineCourseCharacter.h"
#include "UnrealEngineCourseTarget.h"
#include "TP_WeaponComponent.h"

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

	static ConstructorHelpers::FClassFinder<AActor> WeaponPickupClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_Pickup_Base"));
	WeaponPickupClass = WeaponPickupClassFinder.Class;
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

		AUnrealEngineCourseCharacter* Character = Cast<AUnrealEngineCourseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		{
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

		{
			TArray<AActor*> WeaponPickups;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), WeaponPickupClass, WeaponPickups);

			for (AActor* WeaponPickup : WeaponPickups)
			{
				FWeaponPickupMemento Memento;
				
				WeaponPickup->GetName(Memento.Name);
				Memento.bAttached = (Character->GetAttachedWeapon() == WeaponPickup->GetComponentByClass(UTP_WeaponComponent::StaticClass()));

				SaveGameInstance->WeaponPickups.Add(Memento);
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

bool AUnrealEngineCourseGameMode::DoesSaveGameExist() const
{
	UUnrealEngineCourseSaveGameSystem* System = GetGameInstance()->GetSubsystem<UUnrealEngineCourseSaveGameSystem>();
	return System->DoesSaveGameExist();
}

namespace
{

	template <typename InMemento>
	void RestoreActorsByName(UWorld* World, TSubclassOf<AActor> ClassType, const TArray<InMemento>& Mementos)
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(World, ClassType, Actors);

		for (AActor* Actor : Actors)
		{
			// TODO Consider optimizing by sorting the array prior to saving for faster lookups
			const auto* ActorMemento = Mementos.FindByPredicate([Actor](const auto& Memento) {
				return Memento.Name == Actor->GetName();
			});

			if (ActorMemento == nullptr)
			{
				Actor->Destroy();
			}
		}
	}

} // namespace

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

	// TODO Given that AmmoPickups are a BluePrint type, consider serializing the UObject instance
	//		to and FArchive (only properties which are decorated as SaveGame)

	RestoreActorsByName(GetWorld(), AmmoPickupClass, SaveGame->AmmoPickups);
	RestoreActorsByName(GetWorld(), WeaponPickupClass, SaveGame->WeaponPickups);
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
			UUnrealEngineCourseSaveGame* SaveGame = System->GetLatestSaveState();

			// Load Transform and AmmoCount
			Character->Load(SaveGame->Player);

			// Load AttachedWeapon
			for (const FWeaponPickupMemento& WeaponPickup : SaveGame->WeaponPickups)
			{
				if (WeaponPickup.bAttached)
				{
					TArray<AActor*> WeaponPickups;
					UGameplayStatics::GetAllActorsOfClass(GetWorld(), WeaponPickupClass, WeaponPickups);

					AActor** PickupToAttach = WeaponPickups.FindByPredicate([&WeaponPickup](AActor* Value) {
						return Value->GetName() == WeaponPickup.Name;
					});

					if (PickupToAttach != nullptr)
					{
						UTP_WeaponComponent* Component = Cast<UTP_WeaponComponent>((*PickupToAttach)->GetComponentByClass(UTP_WeaponComponent::StaticClass()));
						Component->AttachWeapon(Character);
					}
				}
			}

		}
	}
}
