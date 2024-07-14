// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealEngineCourseProjectileBase.generated.h"

class UProjectileMovementComponent;

UCLASS(Abstract)
class AUnrealEngineCourseProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AUnrealEngineCourseProjectileBase();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Damage)
	float Damage;

	/** Returns Projectile Initial Speed **/
	virtual float GetInitialSpeed() const { return 0.f; }
};
