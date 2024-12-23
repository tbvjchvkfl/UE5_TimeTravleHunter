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

USTRUCT()
struct FST_MovementAnim
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Walk")
	UAnimSequenceBase *WalkStartForward;

	UPROPERTY(EditAnywhere, Category = "Walk")
	UAnimSequenceBase *WalkStartLeft_90;

	UPROPERTY(EditAnywhere, Category = "Walk")
	UAnimSequenceBase *WalkStartLeft_180;

	UPROPERTY(EditAnywhere, Category = "Walk")
	UAnimSequenceBase *WalkStartRight_90;

	UPROPERTY(EditAnywhere, Category = "Walk")
	UAnimSequenceBase *WalkStartRight_180;

	UPROPERTY(EditAnywhere, Category = "Walk")
	UAnimSequenceBase *WalkEnd;

	UPROPERTY(EditAnywhere, Category = "Jog")
	UAnimSequenceBase *JogStartForward;

	UPROPERTY(EditAnywhere, Category = "Jog")
	UAnimSequenceBase *JogStartLeft_90;

	UPROPERTY(EditAnywhere, Category = "Jog")
	UAnimSequenceBase *JogStartLeft_180;

	UPROPERTY(EditAnywhere, Category = "Jog")
	UAnimSequenceBase *JogStartRight_90;

	UPROPERTY(EditAnywhere, Category = "Jog")
	UAnimSequenceBase *JogStartRight_180;

	UPROPERTY(EditAnywhere, Category = "Jog")
	UAnimSequenceBase *JogEnd;

	UPROPERTY(EditAnywhere, Category = "Pivot Turn")
	UAnimSequenceBase *PivotTurn_Left90;

	UPROPERTY(EditAnywhere, Category = "Pivot Turn")
	UAnimSequenceBase *PivotTurn_Left180;

	UPROPERTY(EditAnywhere, Category = "Pivot Turn")
	UAnimSequenceBase *PivotTurn_Right90;

	UPROPERTY(EditAnywhere, Category = "Pivot Turn")
	UAnimSequenceBase *PivotTurn_Right180;
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
	FST_MovementAnim MovementAnimStruct;

	UPROPERTY(EditAnywhere, Category = "Animation | Traversal")
	UAnimMontage *CrouchVaulting_Anim;

	UPROPERTY(EditAnywhere, Category = "Animation | Traversal")
	UAnimMontage *Hurdling_Anim;

	UPROPERTY(EditAnywhere, Category = "Animation | Traversal")
	UAnimMontage *Climbing_Anim;

	UPROPERTY(EditAnywhere, Category = "Animation | Traversal")
	UAnimMontage *Mantling_Anim;

	UPROPERTY(EditAnywhere, Category = "Animation | Action")
	UAnimMontage *Assasination_Anim;

	
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Movement Data", meta = (AllowPrivateAccess = "true"))
	FVector CharacterVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Movement Data", meta = (AllowPrivateAccess = "true"))
	FVector PreviousUnitVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Movement Data", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Movement Data", meta = (AllowPrivateAccess = "true"))
	float MovementElapsedTime;

	// Character Rotation Data
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Rotation Data", meta = (AllowPrivateAccess = "true"))
	float MovementYawDelta;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Rotation Data", meta = (AllowPrivateAccess = "true"))
	float MovementStartAngle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Rotation Data", meta = (AllowPrivateAccess = "true"))
	float PivotTurnAngle;

	// Character Locomotion State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion State", meta = (AllowPrivateAccess = "true"))
	bool bIsAccelation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion State", meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion State", meta = (AllowPrivateAccess = "true"))
	bool bIsCrouch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion State", meta = (AllowPrivateAccess = "true"))
	bool bIsJog;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion State", meta = (AllowPrivateAccess = "true"))
	bool bIsSprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion State", meta = (AllowPrivateAccess = "true"))
	bool bIsParkour;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion State", meta = (AllowPrivateAccess = "true"))
	bool bIsTurn;

	// Character Locomotion SubState
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion SubState", meta = (AllowPrivateAccess = "true"))
	bool CanWalkStartSkip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion SubState", meta = (AllowPrivateAccess = "true"))
	bool CanJogStartSkip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion SubState", meta = (AllowPrivateAccess = "true"))
	float PlayRate;

	// Character AnimSequence Data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterState|Character AnimSequence Data", meta = (AllowPrivateAccess = "true"))
	UAnimSequenceBase *DesiredMoveStartAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterState|Character AnimSequence Data", meta = (AllowPrivateAccess = "true"))
	UAnimSequenceBase *DesiredMoveEndAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterState|Character AnimSequence Data", meta = (AllowPrivateAccess = "true"))
	UAnimSequenceBase *DesiredPivotTurnAnim;

	// Foot IK
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterState|Foot IK", meta = (AllowPrivateAccess = "true"))
	float CurrentInterpSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Foot IK", meta = (AllowPrivateAccess = "true"))
	float Pelvis;

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

	void SetMaxSpeedAndPlayRate();
	void SetRotationRate(float MinLocomotionValue, float MaxLocomotionValue);

	bool SetMovementDirection(float MinValue, float MaxValue, bool Mincluding, bool Maxcluding, float &Direction) const;

	// State Entry Delegate
	void OnEntryMoveStartState(const struct FAnimNode_StateMachine &Machine, int32 PrevStateIndex, int32 NextStateIndex);
	void OnEntryMoveStopState(const struct FAnimNode_StateMachine &Machine, int32 PrevStateIndex, int32 NextStateIndex);
	void OnEntryPivotTurnState(const struct FAnimNode_StateMachine &Machine, int32 PrevStateIndex, int32 NextStateIndex);

	void CheckCurrentDirection();
	void DesiredStartMoveAnim(UAnimSequenceBase *DesiredWalkAnim, UAnimSequenceBase* DesiredJogAnim);

	void FootIK(float DeltaSecond);
	float CapsuleDistance(FName SocketName);
	TTuple<float, FVector> FootTrace(FName SocketName);
};
