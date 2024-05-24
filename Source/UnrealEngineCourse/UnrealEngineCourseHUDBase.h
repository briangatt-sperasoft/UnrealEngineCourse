// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UnrealEngineCourseHUDBase.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class UNREALENGINECOURSE_API UUnrealEngineCourseHUDBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> AmmoCounterLabel;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> AmmoTotalLabel;

	virtual void NativeConstruct() override;
};
