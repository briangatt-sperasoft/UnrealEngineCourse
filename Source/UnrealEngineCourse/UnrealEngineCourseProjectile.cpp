// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealEngineCourseProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "VisualLogger/VisualLogger.h"
#include "Logging/LogMacros.h"

DEFINE_LOG_CATEGORY_STATIC(LogProjectileActor, Verbose, All);

AUnrealEngineCourseProjectile::AUnrealEngineCourseProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AUnrealEngineCourseProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	Damage = 50.0f;
}

void AUnrealEngineCourseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		if (OtherActor->CanBeDamaged())
		{
			AController* controller = nullptr;

			for (AActor* owner = GetOwner(); (owner != nullptr) && (controller == nullptr); owner = owner->GetOwner())
			{
				controller = owner->GetInstigatorController();
			}

			UGameplayStatics::ApplyDamage(OtherActor, Damage, controller, this, UDamageType::StaticClass());
		}

		//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::White, FString::Printf(TEXT("Impulse: %s"), *(GetVelocity() * 100.0f).ToString()));
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();

		UE_VLOG(this, LogProjectileActor, Log, TEXT("Projectile hit at location (%s)"), *Hit.ImpactPoint.ToString());
	}
}
