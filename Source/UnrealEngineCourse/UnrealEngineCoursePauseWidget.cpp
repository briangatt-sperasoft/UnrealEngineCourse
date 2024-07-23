// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealEngineCoursePauseWidget.h"

#include "UnrealEngineCourseGameMode.h"

#include <Components/Button.h>

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

namespace
{

	AUnrealEngineCourseGameMode* GetGameMode(UWorld* World)
	{
		AUnrealEngineCourseGameMode* GameMode = Cast<AUnrealEngineCourseGameMode>(UGameplayStatics::GetGameMode(World));
		
		check(IsValid(GameMode));

		return GameMode;
	}

} // namespace

void UUnrealEngineCoursePauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Resume->OnClicked.AddDynamic(this, &UUnrealEngineCoursePauseWidget::OnResumeClicked);
	Btn_LoadGame->OnClicked.AddDynamic(this, &UUnrealEngineCoursePauseWidget::OnLoadGameClicked);
	Btn_SaveGame->OnClicked.AddDynamic(this, &UUnrealEngineCoursePauseWidget::OnSaveGameClicked);
	Btn_Exit->OnClicked.AddDynamic(this, &UUnrealEngineCoursePauseWidget::OnExitClicked);

	Btn_LoadGame->bIsEnabledDelegate.BindDynamic(this, &UUnrealEngineCoursePauseWidget::IsSaveGameAvailable);
	Btn_LoadGame->SynchronizeProperties();
	//Btn_LoadGame->SetIsEnabled(IsSaveGameAvailable());

	Btn_Resume->SetFocus();
}

void UUnrealEngineCoursePauseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UUnrealEngineCoursePauseWidget::OnResumeClicked()
{
	OnResume.ExecuteIfBound();
}

void UUnrealEngineCoursePauseWidget::OnLoadGameClicked()
{
	GetGameMode(GetWorld())->LoadGame();
}

void UUnrealEngineCoursePauseWidget::OnSaveGameClicked()
{
	GetGameMode(GetWorld())->SaveGame();

	// TODO Hook to async delegate in SaveGame to only enable when async returns success
	Btn_LoadGame->SetIsEnabled(true);
}

void UUnrealEngineCoursePauseWidget::OnExitClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

bool UUnrealEngineCoursePauseWidget::IsSaveGameAvailable()
{
	return GetGameMode(GetWorld())->DoesSaveGameExist();
}
