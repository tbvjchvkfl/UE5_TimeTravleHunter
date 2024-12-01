// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/CachedAnimData.h"
#include "PlayerAnimInstance.generated.h"


class APlayerCharacter;
class UCharacterMovementComponent;
class APlayerCharacterController;
class UAnimMontage;


UENUM()
enum class ECharacterMovementState : uint8
{
	IDLE UMETA(Displayname = "Idle"),
	WALK UMETA(Displayname = "Walk"),
	JOG UMETA(Displayname = "Jog"),
	CROUCH UMETA(Displayname = "Crouch"),
	SPRINT UMETA(Displayname = "Sprint"),
	JUMP UMETA(Displayname = "Jump")
};

UCLASS()
class UE5_TIMETRAVLEHUNTER_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(EditAnywhere, Category = "Animation | Locomotion")
	UAnimMontage *CrouchVaulting_Anim;

	UPROPERTY(EditAnywhere, Category = "Animation | Locomotion")
	UAnimMontage *NormalVaulting_Anim;

	UPROPERTY(EditAnywhere, Category = "Animation | Locomotion")
	UAnimMontage *Hurdling_Anim;

	UPROPERTY(EditAnywhere, Category = "Animation | Action")
	UAnimMontage *Assasination_Anim;

	// MontageDelegate
	FOnMontageEnded VaultMontageDelegate;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void PlayCrouchVaulting();
	void PlayNormalVaulting();
	void PlayHurdling();
	void PlayAssasination();

private:
	//======================================================
	//=					- Variables -					   =
	//======================================================

	//Character Reference
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Reference", meta = (AllowPrivateAccess = "true"))
	APlayerCharacter *OwnerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Reference", meta = (AllowPrivateAccess = "true"))
	UCharacterMovementComponent *OwnerCharacterMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Reference", meta = (AllowPrivateAccess = "true"))
	APlayerCharacterController *OwnerController;

	// Character Movement Data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterState|Movement Data", meta = (AllowPrivateAccess = "true"))
	ECharacterMovementState CharacterMovementState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterState|Movement Data", meta = (AllowPrivateAccess = "true"))
	FVector LastInputVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Movement Data", meta = (AllowPrivateAccess = "true"))
	FVector CharacterVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Movement Data", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed;

	// Character Rotation Data
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Rotation Data", meta = (AllowPrivateAccess = "true"))
	float MovementYawDelta;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Rotation Data", meta = (AllowPrivateAccess = "true"))
	float WalkStartAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Rotation Data", meta = (AllowPrivateAccess = "true"))
	FRotator StartRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Rotation Data", meta = (AllowPrivateAccess = "true"))
	FRotator MainRotation;

	// Character Locomotion State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion State", meta = (AllowPrivateAccess = "true"))
	bool bIsAccelation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion State", meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion State", meta = (AllowPrivateAccess = "true"))
	bool bIsCrouch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="CharacterState|Locomotion State", meta = (AllowPrivateAccess = "true"))
	bool bIsWalk;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion State", meta = (AllowPrivateAccess = "true"))
	bool bIsSprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion State", meta = (AllowPrivateAccess = "true"))
	bool bIsParkour;

	// Character Locomotion SubState
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion SubState", meta = (AllowPrivateAccess = "true"))
	bool CanWalkStartSkip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion SubState", meta = (AllowPrivateAccess = "true"))
	bool CanJogStartSkip;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;

	void SetMovementData();
	void DetermineLocomotionState();
};
