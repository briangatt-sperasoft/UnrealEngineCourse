// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnrealEngineCourseTarget.generated.h"

class UStaticMeshComponent;
struct FTargetMemento;

UCLASS()
class UNREALENGINECOURSE_API AUnrealEngineCourseTarget : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	UStaticMeshComponent* Mesh;
	
public:	
	// Sets default values for this actor's properties
	AUnrealEngineCourseTarget();
	
	UPROPERTY(EditAnywhere, Category=Health, AdvancedDisplay)
	bool bImmortal;
	
	UPROPERTY(EditAnywhere, Category=Health, meta = (EditCondition = "!bImmortal", ClampMin = "0", ClampMax = "100"))
	int32 HitPoints;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void Save(FTargetMemento& memento) const;
	void Load(const FTargetMemento& Memento);

private:
	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

};
