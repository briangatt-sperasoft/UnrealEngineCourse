// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootAbility.h"

#include "WeaponAttributeSet.h"

#include <UnrealEngineCourse/UnrealEngineCourseCharacter.h>
#include <UnrealEngineCourse/TP_WeaponComponent.h>

UShootAbility::UShootAbility()
    : UGameplayAbility()
{
    static ConstructorHelpers::FClassFinder<UGameplayEffect> AmmoUseClassFinder(TEXT("/Game/FirstPerson/Blueprints/GAS/GAE_UseAmmo"));
    CostGameplayEffectClass = AmmoUseClassFinder.Class;
}

bool UShootAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags /*= nullptr*/, const FGameplayTagContainer* TargetTags /*= nullptr*/, OUT FGameplayTagContainer* OptionalRelevantTags /*= nullptr*/) const
{
    const bool bCanActivateAbility = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

    if (!bCanActivateAbility)
    {
        return false;
    }

    check(ActorInfo != nullptr);
    check(ActorInfo->AbilitySystemComponent.IsValid());
    
	const AUnrealEngineCourseCharacter* Character = Cast<AUnrealEngineCourseCharacter>(ActorInfo->OwnerActor);
    const UWeaponAttributeSet* WeaponAttributes = ActorInfo->AbilitySystemComponent->GetSet<UWeaponAttributeSet>();

    return (WeaponAttributes != nullptr) && (WeaponAttributes->GetAmmoCount() > 0.f) &&
		(Character != nullptr) && (Character->GetAttachedWeapon() != nullptr);
}

void UShootAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    //Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	check(CanActivateAbility(Handle, ActorInfo));

	//const AUnrealEngineCourseCharacter* Character = Cast<AUnrealEngineCourseCharacter>(ActorInfo->OwnerActor);
	//Character->GetAttachedWeapon()->Fire();

    const bool bCommitted = CommitAbility(Handle, ActorInfo, ActivationInfo);
    (void)(bCommitted);
    
    EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}