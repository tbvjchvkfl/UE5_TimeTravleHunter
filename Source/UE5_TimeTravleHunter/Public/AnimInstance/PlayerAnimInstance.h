// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"


class APlayerCharacter;
class APlayerCharacterController;

UCLASS()
class UE5_TIMETRAVLEHUNTER_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;

private:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY()
	APlayerCharacter *OwnerCharacter;

	UPROPERTY()
	APlayerCharacterController *OwnerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|LocoMotion", meta = (AllowPrivateAccess = "true"))
	float MovementYawDelta;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|LocoMotion", meta = (AllowPrivateAccess = "true"))
	float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|LocoMotion", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|LocoMotion", meta = (AllowPrivateAccess = "true"))
	bool bIsAccelation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|LocoMotion", meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|LocoMotion", meta = (AllowPrivateAccess = "true"))
	bool bIsCrouch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="CharacterState|LocoMotion", meta = (AllowPrivateAccess = "true"))
	bool bIsWalk;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Action", meta = (AllowPrivateAccess = "true"))
	bool bIsSprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Action", meta = (AllowPrivateAccess = "true"))
	bool bIsAimming;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Action", meta = (AllowPrivateAccess = "true"))
	bool bIsPistol;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Action", meta = (AllowPrivateAccess = "true"))
	bool bIsRifle;
	//======================================================
	//=					- Functionary -					   =
	//======================================================

};
