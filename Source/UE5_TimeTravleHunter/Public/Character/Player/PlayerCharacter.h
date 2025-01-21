// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UStaticMeshComponent;
class UInputMappingContext;
class UInputAction;
class UInventoryComponent;
class UWeaponComponent;
class UItemPoolComponent;
class ATTH_HUD;
class APlayerCharacterController;
class UMotionWarpingComponent;
class UPlayerAnimInstance;
class AEnemyCharacter;
class USceneCaptureComponent2D;

struct FInputActionValue;
struct FInputActionInstance;

UCLASS()
class UE5_TIMETRAVLEHUNTER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	//======================================================
	//=					- Variables -					   =
	//======================================================

	// Interface
	UPROPERTY(VisibleAnywhere, Category = "Character | Interface")
	TScriptInterface<class IInteractionInterface> InteractionInterface;

	UPROPERTY(VisibleAnywhere, Category = "Character | Interface")
	TScriptInterface<class ICharacterActionInterface> ActionInterface;

	// AnimInstance Value
	float MoveElapsedTime;
	
	// WeaponMesh
	UPROPERTY(EditAnywhere, Category = "Character | WeaponMesh")
	UStaticMeshComponent *KatanaMesh_Unarmed;

	UPROPERTY(EditAnywhere, Category = "Character | WeaponMesh")
	UStaticMeshComponent *KatanaCoverMesh;

	UPROPERTY(EditAnywhere, Category = "Character | WeaponMesh")
	UStaticMeshComponent *BowMesh;

	UPROPERTY(EditAnywhere, Category = "Character | WeaponMesh")
	UStaticMeshComponent *SpearMesh;

	UPROPERTY(EditAnywhere, Category = "Character | WeaponMesh")
	UStaticMeshComponent *AssasinKnife;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	APlayerCharacter();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurrentHealth() { return CurrentHealth; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxHealth() { return MaxHealth; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurrentStamina() { return CurrentStamina; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxStamina() { return MaxStamina; };

	FORCEINLINE UInventoryComponent *GetItemInventory() const { return ItemInventory; };
	FORCEINLINE UWeaponComponent *GetWeaponComponent() const { return WeaponComponent; };
	FORCEINLINE UItemPoolComponent *GetItemPoolComponent() const { return ItemPoolComponent; };

protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================

	// Basic Data
	UPROPERTY(EditAnywhere, Category = "Character | Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent *MainCamera;

	UPROPERTY(EditAnywhere, Category = "Character | Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent *CameraBoom;

	// Input
	UPROPERTY(EditAnywhere, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext *DefaultContext;

	UPROPERTY(EditAnywhere, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction *Moving;

	UPROPERTY(EditAnywhere, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction *Jumping;

	UPROPERTY(EditAnywhere, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction *Looking;

	UPROPERTY(EditAnywhere, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction *Sprinting;

	UPROPERTY(EditAnywhere, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction *Crouching;

	UPROPERTY(EditAnywhere, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction *MoveSwitching;

	UPROPERTY(EditAnywhere, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction *Aimming;

	UPROPERTY(EditAnywhere, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction *SwitchingMainWeapon;

	UPROPERTY(EditAnywhere, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction *SwitchingSubWeapon;

	UPROPERTY(EditAnywhere, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction *Interacting;

	UPROPERTY(EditAnywhere, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction *Inventory;

	UPROPERTY(EditAnywhere, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction *NormalAttack;

	UPROPERTY(EditAnywhere, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction *HoldingAction;

	UPROPERTY(EditAnywhere, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction *SpecialAttacking;

	UPROPERTY(EditAnywhere, Category = "Character | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction *Test;

	// Player Property
	UPROPERTY(VisibleAnywhere, Category = "Character | Property")
	ATTH_HUD *OwningHUD;

	UPROPERTY(VisibleAnywhere, Category = "Character | Property")
	UPlayerAnimInstance *OwningAnimInstance;

	UPROPERTY(VisibleAnywhere, Category = "Character | Property")
	APlayerCharacterController *OwningController;

	UPROPERTY(VisibleAnywhere, Category = "Character | Property")
	AEnemyCharacter *EnemyCharacter;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;
	
	// Locomotion
	void Move(const FInputActionInstance &Action);
	void Look(const FInputActionValue &Value);
	void Sprint();
	void DoJump();
	void DoCrouch();
	void WalktoJog();
	void SprintCameraMoving();
	

	// Parkour
	void Parkour();
	void Vaulting();
	void Hurdling();
	void Climbing();
	void InAirMantling();
	void TraversalEnd();

	// Action
	void Assasination();

	void StartAimming();
	void StopAimming();
	void SwitchingWeaponMain();
	void SwitchingWeaponSub();
	
	void BasicAttack();
	void HoldAction();
	void SpecialAttack(const FInputActionInstance& Action);

	void Interaction();
	void ShowInventory();

	void TestFunction();
	
private:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	
	// Character Component
	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory", meta = (AllowPrivateAccess = "true"))
	UInventoryComponent *ItemInventory;

	UPROPERTY(VisibleAnywhere, Category = "Character | Weapon", meta = (AllowPrivateAccess = "true"))
	UWeaponComponent *WeaponComponent;

	UPROPERTY(VisibleAnywhere, Category = "Character | Weapon", meta = (AllowPrivateAccess = "true"))
	UItemPoolComponent *ItemPoolComponent;

	UPROPERTY(VisibleAnywhere, Category = "Character | MotionWarping", meta = (AllowPrivateAccess = "true"))
	UMotionWarpingComponent *CharacterMotionWarping;

	// Character Locomotion
	UPROPERTY(VisibleAnywhere, Category = "Character | Status", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "Character | Status", meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;

	UPROPERTY(VisibleAnywhere, Category = "Character | Status", meta = (AllowPrivateAccess = "true"))
	float MaxStamina;

	UPROPERTY(VisibleAnywhere, Category = "Character | Status", meta = (AllowPrivateAccess = "true"))
	float CurrentStamina;

	UPROPERTY(EditAnywhere, Category = "Character | Status", meta = (AllowPrivateAccess = "true"))
	float LookingRotationValue;

	UPROPERTY(EditAnywhere, Category = "Character | Status", meta = (AllowPrivateAccess = "true"))
	bool DoOnceFlag;

	UPROPERTY(VisibleAnywhere, Category = "Character | Status", meta = (AllowPrivateAccess = "true"))
	FVector VaultStartPos;

	UPROPERTY(VisibleAnywhere, Category = "Character | Status", meta = (AllowPrivateAccess = "true"))
	FVector VaultMiddlePos;

	UPROPERTY(VisibleAnywhere, Category = "Character | Status", meta = (AllowPrivateAccess = "true"))
	FVector VaultLandingPos;

	UPROPERTY(VisibleAnywhere, Category = "Character | Status", meta = (AllowPrivateAccess = "true"))
	FVector MantlePos;

	// Timer
	FTimerHandle ParkourTimerHandle;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	

};
