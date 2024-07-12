// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UnrealEngineCourseSaveGame.generated.h"

class AUnrealEngineCourseProjectileBase;

/**
 * 
 */
UCLASS()
class UNREALENGINECOURSE_API UUnrealEngineCourseSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere)
	FString Map;

	UPROPERTY(VisibleAnywhere)
	FVector Location;

	UPROPERTY(VisibleAnywhere)
	TMap<TSubclassOf<AUnrealEngineCourseProjectileBase>, int32> AmmoCount;
};
