// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnrealEngineCoursePauseWidget.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class UNREALENGINECOURSE_API UUnrealEngineCoursePauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UButton* Btn_Resume;
	
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UButton* Btn_LoadGame;
	
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UButton* Btn_SaveGame;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UButton* Btn_Exit;

protected:
	void NativeConstruct() override;

private:
	UFUNCTION()
	void OnResume();
	
	UFUNCTION()
	void OnLoadGame();
	
	UFUNCTION()
	void OnSaveGame();

	UFUNCTION()
	void OnExit();
	
	UFUNCTION()
	bool IsSaveGameAvailable();
};
