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

	Btn_Resume->OnClicked.AddDynamic(this, &UUnrealEngineCoursePauseWidget::OnResume);
	Btn_LoadGame->OnClicked.AddDynamic(this, &UUnrealEngineCoursePauseWidget::OnLoadGame);
	Btn_SaveGame->OnClicked.AddDynamic(this, &UUnrealEngineCoursePauseWidget::OnSaveGame);
	Btn_Exit->OnClicked.AddDynamic(this, &UUnrealEngineCoursePauseWidget::OnExit);

	Btn_LoadGame->bIsEnabledDelegate.BindDynamic(this, &UUnrealEngineCoursePauseWidget::IsSaveGameAvailable);
}

void UUnrealEngineCoursePauseWidget::OnResume()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void UUnrealEngineCoursePauseWidget::OnLoadGame()
{
	GetGameMode(GetWorld())->LoadGame();
}

void UUnrealEngineCoursePauseWidget::OnSaveGame()
{
	GetGameMode(GetWorld())->SaveGame();
}

void UUnrealEngineCoursePauseWidget::OnExit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

bool UUnrealEngineCoursePauseWidget::IsSaveGameAvailable()
{
	return GetGameMode(GetWorld())->DoesSaveGameExist();
}
