// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "UnrealEngineCourseTarget.generated.h"

class UStaticMeshComponent;
struct FTargetMemento;

UCLASS()
class UNREALENGINECOURSE_API AUnrealEngineCourseTarget : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category=GameplayAbilitySystem)
	UAbilitySystemComponent* AbilitySystem;
	
public:	
	// Sets default values for this actor's properties
	AUnrealEngineCourseTarget();
	
	UPROPERTY(EditAnywhere, Category=Health, AdvancedDisplay)
	bool bImmortal;
	
	UPROPERTY(EditAnywhere, Category=Health, meta = (EditCondition = "!bImmortal", ClampMin = "0", ClampMax = "100"))
	int32 Health;
	
protected:
	// -- AActor overrides

	virtual void PostInitializeComponents() override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// -- IAbilitySystemInterface overrides

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:
	void Save(FTargetMemento& memento) const;
	void Load(const FTargetMemento& Memento);

private:
	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

};
