// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ShootAbility.generated.h"

/**
 * 
 */
UCLASS()
class UNREALENGINECOURSE_API UShootAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	UShootAbility();

	// -- UGameplayAbility overrides --

	/** Returns true if this ability can be activated right now. Has no side effects */
	bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	/** Actually activate ability, do not call this directly */
	void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// -- UGameplayAbility overrides --
};
