// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_PredictProjectileComponent.h"
#include "UnrealEngineCourseProjectileBase.h"
#include "UnrealEngineCourseCharacter.h"
#include "Kismet/GameplayStatics.h"

namespace
{

float GetSpeed(AUnrealEngineCourseProjectileBase* Projectile)
{
	return Projectile->GetInitialSpeed();
}

} // namespace

// Sets default values for this component's properties
UTP_PredictProjectileComponent::UTP_PredictProjectileComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

bool UTP_PredictProjectileComponent::Predict(AUnrealEngineCourseCharacter* Character, AUnrealEngineCourseProjectileBase* Projectile)
{
	if (Projectile == nullptr)
	{
		return false;
	}

	UWorld* const World = GetWorld();
	if (World == nullptr)
	{
		return false;
	}

	FPredictProjectilePathParams PredictParams;

	// TODO A proper implementation would predict the path, generate a spline via the Spline Component
	//		and later render the spline as a particle trail, spline mesh or place meshes along the spline.
	//		Refer to content starter pack for implementation examples.

	PredictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;
	PredictParams.DrawDebugTime = DrawTime;
	PredictParams.MaxSimTime = MaxSimTime;
	PredictParams.SimFrequency = SimFrequency;

	PredictParams.ProjectileRadius = Thickness;
	PredictParams.StartLocation = Projectile->GetActorLocation();

	const FRotator Rotation = Projectile->GetActorRotation();
	PredictParams.LaunchVelocity = Rotation.Vector() * GetSpeed(Projectile);

	PredictParams.bTraceWithCollision = true;
	PredictParams.ActorsToIgnore = { Character, Projectile };

	FPredictProjectilePathResult PredictResult;

	return UGameplayStatics::PredictProjectilePath(World, PredictParams, PredictResult);
}
