// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealEngineCourseTarget.h"

// Sets default values
AUnrealEngineCourseTarget::AUnrealEngineCourseTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HitPoints = 100;
	bImmortal = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetSimulatePhysics(true);

	RootComponent = Mesh;

}

// Called when the game starts or when spawned
void AUnrealEngineCourseTarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnrealEngineCourseTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

