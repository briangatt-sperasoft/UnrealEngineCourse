// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UnrealEngineCourseSaveGame.generated.h"

class AActor;
class AUnrealEngineCourseProjectileBase;

/**
 *
 */
USTRUCT(BlueprintType)
struct FPlayerMemento
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FTransform Transform;

	UPROPERTY(BlueprintReadWrite)
	TMap<TSubclassOf<AUnrealEngineCourseProjectileBase>, int32> AmmoCount;
};

/**
 *
 */
USTRUCT(BlueprintType)
struct FTargetMemento
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString Name;

	UPROPERTY(BlueprintReadWrite)
	FTransform Transform;

	UPROPERTY(BlueprintReadWrite)
	int32 HitPoints;
};

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
	FPlayerMemento Player;

	UPROPERTY(VisibleAnywhere)
	TArray<FTargetMemento> Targets;
};
