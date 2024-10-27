// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealEngineCourseCharacter.h"
#include "UnrealEngineCourseProjectile.h"
#include "UnrealEngineCourseSaveGame.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TP_WeaponComponent.h"
#include "UnrealEngineCoursePauseWidget.h"

namespace
{
	TAutoConsoleVariable<bool> CVarInfiniteAmmo(
		TEXT("r.InfiniteAmmo"),
		false,
		TEXT("Enables/Disable infinite ammo"),
		ECVF_Scalability | ECVF_RenderThreadSafe);
} // namespace

//////////////////////////////////////////////////////////////////////////
// AUnrealEngineCourseCharacter

AUnrealEngineCourseCharacter::AUnrealEngineCourseCharacter()
{
	// Character doesn't have a rifle at start
	bHasRifle = false;
	AttachedWeapon = nullptr;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

void AUnrealEngineCourseCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (Subsystem != nullptr)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AUnrealEngineCourseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUnrealEngineCourseCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUnrealEngineCourseCharacter::Look);

		//Pause
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &AUnrealEngineCourseCharacter::Pause);
	}
}


void AUnrealEngineCourseCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AUnrealEngineCourseCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

namespace
{

	void TogglePauseGame(APlayerController* PC)
	{
		check(PC != nullptr);

		const bool bPaused = PC->IsPaused();

		PC->SetPause(!bPaused);

		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::White, bPaused ? TEXT("Resume") : TEXT("Pause"));
	}

} // namespace

void AUnrealEngineCourseCharacter::Pause(const FInputActionValue& /*Value*/)
{
	APlayerController* const PC = CastChecked<APlayerController>(Controller);

	if ((PC != nullptr) && PC->IsLocalPlayerController())
	{
		if (PauseWidgetClass)
		{
			if (PauseWidget == nullptr)
			{
				PauseWidget = CreateWidget<UUnrealEngineCoursePauseWidget>(PC, PauseWidgetClass);

				PauseWidget->OnResume.BindLambda([this, PC]() {

					//FInputModeGameOnly Mode;
					//PC->SetInputMode(Mode);

					//PauseWidget->RemoveFromViewport();
					PauseWidget->RemoveFromParent();

					PauseWidget = nullptr;
					PC->bShowMouseCursor = false;

					TogglePauseGame(PC);

				});

				//FInputModeUIOnly Mode;
				//Mode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
				//PC->SetInputMode(Mode);

				PauseWidget->AddToViewport();
				
				PC->bShowMouseCursor = true;
			}
			else
			{
				PauseWidget->OnResume.Execute();
				return;
			}
		}

		TogglePauseGame(PC);
	}
}

void AUnrealEngineCourseCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AUnrealEngineCourseCharacter::GetHasRifle()
{
	return bHasRifle;
}

namespace
{

	const int32* GetAmmo(const TMap<TSubclassOf<AUnrealEngineCourseProjectileBase>, int32>& AmmoRegistry, TSubclassOf<AUnrealEngineCourseProjectileBase> ProjectileType)
	{
		return AmmoRegistry.Find(ProjectileType);
	}

	bool SetAmmo(TMap<TSubclassOf<AUnrealEngineCourseProjectileBase>, int32>& AmmoRegistry, TSubclassOf<AUnrealEngineCourseProjectileBase> ProjectileType, int32 AmmoCount)
	{
		if (ProjectileType == nullptr)
		{
			return false;
		}

		int32& Ammo = AmmoRegistry.FindOrAdd(ProjectileType);
		Ammo = AmmoCount;

		return true;
	}

} // namespace

bool AUnrealEngineCourseCharacter::UpdateAmmo(int32 Diff, TSubclassOf<AUnrealEngineCourseProjectileBase> ProjectileType)
{
	const bool bInfiniteAmmo = CVarInfiniteAmmo->GetBool();

	if (bInfiniteAmmo)
	{
		// Assume updated successfully, even though the value has not changed
		return true;
	}

	if (ProjectileType == nullptr)
	{
		return false;
	}

	int32 Count = GetAmmoCount(ProjectileType);

	const int32 CurrentBulletCount = Count;
	Count = std::max(Count + Diff, 0);

	const bool bUpdated = (CurrentBulletCount != Count);

	if (bUpdated)
	{
		SetAmmo(AmmoCount, ProjectileType, Count);

		if ((AttachedWeapon == nullptr) || (AttachedWeapon->ProjectileClass == ProjectileType)) {
			OnAmmoUpdated.Broadcast(this, Count, ProjectileType);
		}
	}
	
	return bUpdated;
}

int32 AUnrealEngineCourseCharacter::GetAmmoCount(TSubclassOf<AUnrealEngineCourseProjectileBase> ProjectileType /*= nullptr*/) const
{
	if (ProjectileType == nullptr)
	{
		ProjectileType = AUnrealEngineCourseProjectile::StaticClass();
	}

	const int32* WeaponAmmo = GetAmmo(AmmoCount, ProjectileType);
	return (WeaponAmmo != nullptr) ? *WeaponAmmo : 0;
}

UTP_WeaponComponent* AUnrealEngineCourseCharacter::GetAttachedWeapon() const
{
	return AttachedWeapon;
}

void AUnrealEngineCourseCharacter::Save(FPlayerMemento& SaveGame) const
{
	SaveGame.AmmoCount = AmmoCount;
	SaveGame.Transform = GetActorTransform();
}

void AUnrealEngineCourseCharacter::Load(const FPlayerMemento& SaveGame)
{
	int CurrentBulletCount = GetAmmoCount();

	SetActorTransform(SaveGame.Transform);
	AmmoCount = SaveGame.AmmoCount;
}

void AUnrealEngineCourseCharacter::AttachWeapon(UTP_WeaponComponent* Weapon)
{
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);

	if (AttachedWeapon != nullptr)
	{
		FDetachmentTransformRules DetachmentRules(AttachmentRules, true);
		AttachedWeapon->DetachFromComponent(DetachmentRules);
		
		AttachedWeapon->GetOwner()->Destroy();
	}

	// Attach the weapon to the First Person Character
	Weapon->AttachToComponent(GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// switch bHasRifle so the animation blueprint can switch to another animation set
	SetHasRifle(Weapon->bIsRifle);

	AttachedWeapon = Weapon;

	OnAmmoUpdated.Broadcast(this, GetAmmoCount(Weapon->ProjectileClass), Weapon->ProjectileClass);
}
