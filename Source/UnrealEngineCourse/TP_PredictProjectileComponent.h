// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TP_PredictProjectileComponent.generated.h"

class AUnrealEngineCourseProjectileBase;
class AUnrealEngineCourseCharacter;

// NOTE This concept might have better been suited as a UINTERFACE instead (implement Predict).
//		We opted for a component for learning purposes and to share the common implementation.

// NOTE For Within attribute, the U/A prefix needs to be dropped
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALENGINECOURSE_API UTP_PredictProjectileComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTP_PredictProjectileComponent();

	UPROPERTY(EditDefaultsOnly, Category = "Render")
	float DrawTime = 6.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Render")
	float Thickness = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Simulation")
	float SimFrequency = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Simulation")
	float MaxSimTime = 100.f;

	UFUNCTION(BlueprintCallable)
	bool Predict(AUnrealEngineCourseCharacter* Character, AUnrealEngineCourseProjectileBase* Projectile);
};
