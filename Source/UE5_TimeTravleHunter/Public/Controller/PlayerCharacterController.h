// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

class APlayerCharacter;
class ATTH_HUD;
class UInputMappingContext;
class UInputAction;

struct FInputActionValue;

UCLASS()
class UE5_TIMETRAVLEHUNTER_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(EditAnywhere, Category = "Controller | Base")
	TSubclassOf<APlayerCharacter>OwnerCharacter;

	UPROPERTY(EditAnywhere, Category = "Controller | Property")
	float LookingRotationValue;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	APlayerCharacterController();

protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================



	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void BeginPlay()override;
	virtual void Tick(float DeltaTime)override;
	virtual void SetupInputComponent()override;
private:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(VisibleAnywhere, Category = "Controller | Status", meta = (AllowPrivateAccess = "true"))
	APlayerCharacter *PlayerCharacter;

	UPROPERTY()
	ATTH_HUD *OwningHUD;

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

	//======================================================
	//=					- Functionary -					   =
	//======================================================

	// Input Function
	void Move(const FInputActionValue &Value);
	void Look(const FInputActionValue &Value);
	void Sprint(const FInputActionValue &Value);
	void Crouch(const FInputActionValue &Value);
	void WalktoJog(const FInputActionValue &Value);
	void JumpStart();
	void JumpStop();


	void StartAimming();
	void StopAimming();
	void SwitchingWeaponMain();
	void SwitchingWeaponSub();
	void SprintCameraMoving();

	void Interaction();
	void ShowInventory();

	void TestFunction();
};
