// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UnrealEngineCourseSaveGame.generated.h"

class AUnrealEngineCourseProjectileBase;

/**
 *
 */
USTRUCT(BlueprintType)
struct FTransformMemento
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector Location;

	UPROPERTY(BlueprintReadWrite)
	FRotator Rotation;
};

/**
 *
 */
USTRUCT(BlueprintType)
struct FPlayerMemento
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FTransformMemento Transform;

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
	FTransformMemento Transform;

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
