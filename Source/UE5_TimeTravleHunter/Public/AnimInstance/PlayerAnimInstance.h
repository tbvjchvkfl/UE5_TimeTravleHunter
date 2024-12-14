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
struct FRigElementKey;

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

USTRUCT()
struct FST_FootIK
{
	GENERATED_USTRUCT_BODY()
	float Offset;
	FVector HitLocation;
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
	UAnimMontage *Hurdling_Anim;

	UPROPERTY(EditAnywhere, Category = "Animation | Locomotion")
	UAnimMontage *Climbing_Anim;

	UPROPERTY(EditAnywhere, Category = "Animation | Locomotion")
	UAnimMontage *Mantling_Anim;

	UPROPERTY(EditAnywhere, Category = "Animation | Action")
	UAnimMontage *Assasination_Anim;

	UPROPERTY(EditAnywhere, Category = "Animation | Test")
	UAnimSequence *TestSeq;

	UPROPERTY(EditAnywhere, Category = "Animation | Test")
	UAnimSequenceBase *TestBase;
	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void PlayCrouchVaulting();
	void PlayHurdling();
	void PlayClimbing();
	void PlayMantling();
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
	bool bIsJog;

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

	// Foot IK
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterState|Foot IK", meta = (AllowPrivateAccess = "true"))
	float CurrentInterpSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Foot IK", meta = (AllowPrivateAccess = "true"))
	float Displacement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Foot IK", meta = (AllowPrivateAccess = "true"))
	FRotator L_FootRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Foot IK", meta = (AllowPrivateAccess = "true"))
	FRotator R_FootRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Foot IK", meta = (AllowPrivateAccess = "true"))
	float L_FootIK;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Foot IK", meta = (AllowPrivateAccess = "true"))
	float R_FootIK;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void InitAnimationInstance();
	void SetMovementData();

	void SetMaxAccelAndPlayRate();
	void SetRotationRate(float MinLocomotionValue, float MaxLocomotionValue);

	bool SetMovementDirection(float MinValue, float MaxValue, bool Mincluding, bool Maxcluding, float &Direction) const;
	void DetermineMovementDirection();

	void FootIK(float DeltaSecond);
	TTuple<bool, float, FVector> FootTrace(FName SocketName);
	TTuple<bool, float>CapsuleDistance(FName SocketName);
};
