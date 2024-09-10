// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UInventoryComponent;

struct FInputActionValue;

UCLASS()
class UE5_TIMETRAVLEHUNTER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	//======================================================
	//=					- Variables -					   =
	//======================================================


	

	// Character Locomotion
	UPROPERTY(VisibleAnywhere, Category = "Character | Status")
	bool bIsWalk;

	UPROPERTY(VisibleAnywhere, Category = "Character | Status")
	bool bIsSprint;

	UPROPERTY(VisibleAnywhere, Category = "Character | Status")
	bool bIsCrouch;

	UPROPERTY(VisibleAnywhere, Category = "Character | Status")
	bool bIsAimming;

	UPROPERTY(VisibleAnywhere, Category = "Character | Status")
	bool bIsPistol;

	UPROPERTY(VisibleAnywhere, Category = "Character | Status")
	bool bIsRifle;

	UPROPERTY(VisibleAnywhere, Category = "Character | Status")
	bool bIsShotgun;
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

	UFUNCTION()
	FORCEINLINE UInventoryComponent* GetItemInventory() { return ItemInventory; }

protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================

	// Component
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

	// Inventory
	UPROPERTY(EditAnywhere, Category = "Character | Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<int> TestInventory;
	//TArray<AWeaponBase> WeaponInventory;

	//UPROPERTY(EditAnywhere, Category = "Character | Inventory", meta = (AllowPrivateAccess = "true"))
	//AWeaponBase *EquipWeapon;

	int TestEquip;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	// Input Function
	void Move(const FInputActionValue &Value);
	void Look(const FInputActionValue &Value);
	void Sprint(const FInputActionValue &Value);
	void Crouch(const FInputActionValue &Value);
	void WalktoJog(const FInputActionValue &Value);

	void StartAimming();
	void StopAimming();
	void SwitchingWeaponMain();
	void SwitchingWeaponSub();
	void SprintCameraMoving();

	void Interaction();

	void TestFunction();
private:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	
	// Character Component
	UPROPERTY(VisibleAnywhere, Category = "Character | Inventory")
	UInventoryComponent *ItemInventory;

	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<class IInteractionInterface> InteractionInterface;

	// Character Locomotion
	UPROPERTY(VisibleAnywhere, Category = "Character | Status", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "Character | Status", meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;

	UPROPERTY(VisibleAnywhere, Category = "Character | Status", meta = (AllowPrivateAccess = "true"))
	float MaxStamina;

	UPROPERTY(VisibleAnywhere, Category = "Character | Status", meta = (AllowPrivateAccess = "true"))
	float CurrentStamina;

	UPROPERTY(EditAnywhere, Category = "Character | Property", meta = (AllowPrivateAccess = "true"))
	float LookingRotationValue;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	

};
