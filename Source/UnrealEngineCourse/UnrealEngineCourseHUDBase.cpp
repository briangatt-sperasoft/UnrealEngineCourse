// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealEngineCourseHUDBase.h"

#include "UnrealEngineCourseCharacter.h"

#include <Kismet/GameplayStatics.h>

#include <Components/TextBlock.h>

namespace
{
	AUnrealEngineCourseCharacter* GetPlayerCharacter(const UUserWidget& widget, int32 playerIndex = 0)
	{
		ACharacter* character = UGameplayStatics::GetPlayerCharacter(widget.GetWorld(), playerIndex);
		return Cast<AUnrealEngineCourseCharacter>(character);
	}

	FText GetBulletCount(const AUnrealEngineCourseCharacter& character)
	{
		return FText::AsNumber(character.GetAmmoCount());
	}
}

void UUnrealEngineCourseHUDBase::NativeConstruct()
{
	AUnrealEngineCourseCharacter* character = GetPlayerCharacter(*this);

	const FText value = (character != nullptr) ? GetBulletCount(*character) : FText::FromString("999");

	AmmoCounterLabel->SetText(value);

	// NOTE AmmoTotalLabel is an optional widget, hence the null check
	if (AmmoTotalLabel != nullptr)
	{
		AmmoTotalLabel->SetText(value);
	}
}
