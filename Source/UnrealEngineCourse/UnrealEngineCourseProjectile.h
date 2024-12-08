// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "UnrealEngineCourseProjectileBase.h"
#include "UnrealEngineCourseProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UGameplayEffect;

UCLASS(config=Game)
class AUnrealEngineCourseProjectile : public AUnrealEngineCourseProjectileBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleDefaultsOnly, Category = GameplayAbilitySystem)
	UAbilitySystemComponent* AbilitySystem;

	UPROPERTY(EditAnywhere, Category = GameplayAbilitySystem)
	TSubclassOf<UGameplayEffect> DamageEffect;

public:
	AUnrealEngineCourseProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	float GetInitialSpeed() const override;

	// -- IAbilitySystemInterface overrides -- 
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// -- IAbilitySystemInterface overrides -- 
};

