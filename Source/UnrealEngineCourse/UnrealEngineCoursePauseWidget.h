// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnrealEngineCoursePauseWidget.generated.h"

class UButton;

DECLARE_DELEGATE(FnOnResumeEvent);

/**
 * 
 */
UCLASS()
class UNREALENGINECOURSE_API UUnrealEngineCoursePauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* Btn_Resume = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* Btn_LoadGame = nullptr;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* Btn_SaveGame = nullptr;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UButton* Btn_Exit = nullptr;

	FnOnResumeEvent OnResume;

protected:
	void NativeConstruct() override;

	void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnResumeClicked();
	
	UFUNCTION()
	void OnLoadGameClicked();
	
	UFUNCTION()
	void OnSaveGameClicked();

	UFUNCTION()
	void OnExitClicked();
	
	UFUNCTION()
	bool IsSaveGameAvailable();
};
