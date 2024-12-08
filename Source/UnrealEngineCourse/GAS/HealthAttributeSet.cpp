// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthAttributeSet.h"

#include "Logging/LogMacros.h"

void UHealthAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
    Super::PostAttributeChange(Attribute, OldValue, NewValue);

    UE_LOG(LogTemp, Log, TEXT("%s: %f -> %f"), *Attribute.GetName(), OldValue, NewValue);
}