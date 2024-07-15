// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealEngineCourseTarget.h"
#include "UnrealEngineCourseSaveGame.h"

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

	OnTakeAnyDamage.AddDynamic(this, &AUnrealEngineCourseTarget::OnTakeDamage);
}

void AUnrealEngineCourseTarget::Save(FTargetMemento& Memento) const
{
	GetName(Memento.Name);

	Memento.Transform = GetActorTransform();
	Memento.HitPoints = HitPoints;
}

void AUnrealEngineCourseTarget::Load(const FTargetMemento& Memento)
{
	SetActorTransform(Memento.Transform);
	HitPoints = Memento.HitPoints;
}

void AUnrealEngineCourseTarget::OnTakeDamage(AActor* /*DamagedActor*/, float Damage, const class UDamageType* /*DamageType*/, AController* /*InstigatedBy*/, AActor* /*DamageCauser*/)
{
	if (!bImmortal)
	{
		HitPoints -= static_cast<int>(Damage);
	}

	if (HitPoints <= 0)
	{
		Destroy();
	}
}
