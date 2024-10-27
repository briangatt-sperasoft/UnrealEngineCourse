// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "UnrealEngineCourseCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

class AUnrealEngineCourseCharacter;
class UTP_WeaponComponent;
class AUnrealEngineCourseProjectileBase;
class UUnrealEngineCoursePauseWidget;
struct FPlayerMemento;

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAmmoUpdateDelegateSignature, AUnrealEngineCourseCharacter*, Character, int32, BulletCount, TSubclassOf<AUnrealEngineCourseProjectileBase>, ProjectileType);

UCLASS(config=Game)
class AUnrealEngineCourseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Pause Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PauseAction;

	// TODO Ideally, AmmoCount is modeled as part of a 'Magazine' which is owned by a 'Weapon'
	//		A character may own an 'Inventory' which hosts 'Magazines'

	// TODO Consider moving AmmoCount or similar properties to PlayerState instead
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TMap<TSubclassOf<AUnrealEngineCourseProjectileBase>, int32> AmmoCount;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	UTP_WeaponComponent* AttachedWeapon;

	UPROPERTY()
	UUnrealEngineCoursePauseWidget* PauseWidget;

	UPROPERTY(VisibleDefaultsOnly, Category = GameplayAbilitySystem)
	UAbilitySystemComponent* AbilitySystem;

public:
	AUnrealEngineCourseCharacter();

protected:
	virtual void BeginPlay();

	// -- IAbilitySystemInterface overrides

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;
	
	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();
	
	/** Delegate notified once the weapon fires a bullet */
	UPROPERTY(BlueprintAssignable, Category = Ammo)
	FOnAmmoUpdateDelegateSignature OnAmmoUpdated;
	
	/** Updates the ammo count with the noted difference */
	UFUNCTION(BlueprintCallable, Category = Ammo)
	bool UpdateAmmo(int32 Diff, TSubclassOf<AUnrealEngineCourseProjectileBase> ProjectileType);
	
	UFUNCTION(BlueprintPure, Category = Ammo)
	int32 GetAmmoCount(TSubclassOf<AUnrealEngineCourseProjectileBase> ProjectileType = nullptr) const;

	UFUNCTION(BlueprintPure, Category = Weapon)
	UTP_WeaponComponent* GetAttachedWeapon() const;

	void Save(FPlayerMemento& SaveGame) const;
	void Load(const FPlayerMemento& SaveGame);

	void AttachWeapon(UTP_WeaponComponent* Weapon);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
	TSubclassOf<UUnrealEngineCoursePauseWidget> PauseWidgetClass;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for pausing input */
	void Pause(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }


};

