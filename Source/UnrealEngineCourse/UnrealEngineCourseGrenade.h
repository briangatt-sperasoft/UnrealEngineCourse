// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealEngineCourseProjectileBase.h"
#include "UnrealEngineCourseGrenade.generated.h"

class UCapsuleComponent;
class UProjectileMovementComponent;
class URadialForceComponent;

/**
 * 
 */
UCLASS()
class UNREALENGINECOURSE_API AUnrealEngineCourseGrenade : public AUnrealEngineCourseProjectileBase
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	UCapsuleComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	URadialForceComponent* RadialForce;
	
public:
	AUnrealEngineCourseGrenade();
	
	/** Delay (in seconds) till the grenade detonates */
	UPROPERTY(EditAnywhere, Category = Weapon)
	float DetonateDelay;
	
	// AActor overrides
	void BeginPlay() override;
	// AActor overrides

	float GetInitialSpeed() const override;

private:
	void OnDetonate();
};
