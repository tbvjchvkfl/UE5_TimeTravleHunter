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

UENUM()
enum class ECharacterMovementDirection : uint8
{
	FOWARD UMETA(DisplayName = "Foward"),
	TURNLEFT_90 UMETA(DisplayName = "TurnLeft_90"),
	TURNLEFT_180 UMETA(DisplayName = "TurnLeft_180"),
	TURNRIGHT_90 UMETA(DisplayName = "TurnRight_90"),
	TURNRIGHT_180 UMETA(DisplayName = "TurnRight_180")
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Movement Data", meta = (AllowPrivateAccess = "true"))
	FVector CharacterVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Movement Data", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Movement Data", meta = (AllowPrivateAccess = "true"))
	float ActualGroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Movement Data", meta = (AllowPrivateAccess = "true"))
	FVector PreviousLocation;

	// Character Rotation Data
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Rotation Data", meta = (AllowPrivateAccess = "true"))
	ECharacterMovementDirection CharacterMovementDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Rotation Data", meta = (AllowPrivateAccess = "true"))
	float MovementYawDelta;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Rotation Data", meta = (AllowPrivateAccess = "true"))
	float MovementStartAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Rotation Data", meta = (AllowPrivateAccess = "true"))
	float DeltaTime;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion SubState", meta = (AllowPrivateAccess = "true"))
	float PlayRate;
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void InitAnimationInstance();
	void SetMovementData();
	void DetermineLocomotionState();

	void SetMaxAccelAndPlayRate();
	void SetRotationRate(float MinLocomotionValue, float MaxLocomotionValue);

	bool SetMovementDirection(float MinValue, float MaxValue, float &Direction) const;
	UFUNCTION(BlueprintCallable)
	void DetermineMovementDirection();
};
