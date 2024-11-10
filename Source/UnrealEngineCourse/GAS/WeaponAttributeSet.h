// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "WeaponAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class UNREALENGINECOURSE_API UWeaponAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData AmmoCount;

public:
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UWeaponAttributeSet, AmmoCount)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(AmmoCount)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(AmmoCount)
	GAMEPLAYATTRIBUTE_VALUE_INITTER(AmmoCount)
};
