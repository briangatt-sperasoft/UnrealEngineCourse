// Fill out your copyright notice in the Description page of Project Settings.


#include "UnrealEngineCourseGrenade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AUnrealEngineCourseGrenade::AUnrealEngineCourseGrenade()
{
	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CollisionComp->SetCapsuleSize(5.0f, 10.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->bApplyImpulseOnDamage = true;
	
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("ForceComp"));
	RadialForce->Radius = 200.f;
	RadialForce->bIgnoreOwningActor = true;
	RadialForce->bImpulseVelChange = true;
	RadialForce->ImpulseStrength = 4000.f;
	RadialForce->ForceStrength = 0.f;

	RadialForce->SetupAttachment(CollisionComp);

	DetonateDelay = 3.0f;
	Damage = 80.0f;
}

void AUnrealEngineCourseGrenade::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &AUnrealEngineCourseGrenade::OnDetonate, DetonateDelay, false);
}

void AUnrealEngineCourseGrenade::OnDetonate()
{
	DrawDebugSphere(GetWorld(), GetActorLocation(), RadialForce->Radius, 24, FColor::Red, false, 5.f, 0, 1.0f);

	AController* Controller = nullptr;
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), RadialForce->Radius, UDamageType::StaticClass(), { this }, this, Controller, true);
	
	RadialForce->FireImpulse();

	Destroy();
}
