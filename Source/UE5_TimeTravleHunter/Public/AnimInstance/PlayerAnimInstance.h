// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/CachedAnimData.h"
#include "PlayerAnimInstance.generated.h"


class APlayerCharacter;
class APlayerCharacterController;
class UAnimMontage;
struct FCachedAnimStateData;

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
	APlayerCharacterController *OwnerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Reference", meta = (AllowPrivateAccess = "true"))
	ECharacterMovementState CharacterMovementState;

	// Character Locomotion Data
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion Data", meta = (AllowPrivateAccess = "true"))
	FVector CharacterVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion Data", meta = (AllowPrivateAccess = "true"))
	float MovementYawDelta;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion Data", meta = (AllowPrivateAccess = "true"))
	float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion Data", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed;

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
	bool bIsAimming;

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

private:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	/*UPROPERTY()
	APlayerCharacter *OwnerCharacter;

	UPROPERTY()
	APlayerCharacterController *OwnerController;

	UPROPERTY()
	float DeltaX_Time;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|LocoMotion", meta = (AllowPrivateAccess = "true"))
	ECharacterMovementState CharacterMovementState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|LocoMotion", meta = (AllowPrivateAccess = "true"))
	FVector CharacterVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|LocoMotion", meta = (AllowPrivateAccess = "true"))
	float MovementYawDelta;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|LocoMotion", meta = (AllowPrivateAccess = "true"))
	float Direction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|LocoMotion", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|LocoMotion", meta = (AllowPrivateAccess = "true"))
	float MaxSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|LocoMotion", meta = (AllowPrivateAccess = "true"))
	FVector LastInputVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|LocoMotion", meta = (AllowPrivateAccess = "true"))
	bool bIsAccelation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|LocoMotion", meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|LocoMotion", meta = (AllowPrivateAccess = "true"))
	bool bIsCrouch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|LocoMotion", meta = (AllowPrivateAccess = "true"))
	bool bIsWalk;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Action", meta = (AllowPrivateAccess = "true"))
	bool bIsSprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Action", meta = (AllowPrivateAccess = "true"))
	bool bIsAimming;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Action", meta = (AllowPrivateAccess = "true"))
	bool bIsParkour;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Action", meta = (AllowPrivateAccess = "true"))
	float PlayRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Action", meta = (AllowPrivateAccess = "true"))
	FRotator StartRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Action", meta = (AllowPrivateAccess = "true"))
	FRotator PrimaryTargetRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Action", meta = (AllowPrivateAccess = "true"))
	FRotator SecondaryTargetRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Action", meta = (AllowPrivateAccess = "true"))
	float WalkStartRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Action", meta = (AllowPrivateAccess = "true"))
	float JogStartRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterState|Action", meta = (AllowPrivateAccess = "true"))
	FCachedAnimStateData WalkStateData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterState|Action", meta = (AllowPrivateAccess = "true"))
	FCachedAnimStateData JogStateData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterState|Action", meta = (AllowPrivateAccess = "true"))
	FCachedAnimStateData LocomotionStateData;

	bool bIsPlayWalkStart;
	bool bIsPlayJogStart;
	bool DoInputVectorRotation;*/
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	/*virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;
	virtual void NativePostEvaluateAnimation()override;

	float SafeDivide(float Numerator, float Denominator, float DefaultValue = 0)const;

	void SetMovementData();
	bool IsMovementWithInThresholds(float MinCurrentSpeed, float MinMaxSpeed, float MinInputAccelation) const;
	void DetermineLocomotionState();
	void TrackLocomotionState(ECharacterMovementState LocomotionState);

	void ResetTargetRotation();
	void UpdateLocomotionValue();
	void UpdateOnWalkEntry();
	void UpdateOnJogEntry();

	void UpdateCharacterRotation();
	FRotator GetTargetRotation() const;
	void ResetTransition();*/
};
