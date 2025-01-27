// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

class AEnemyCharacter;
class AEnemyCharacterController;
class UCharacterMovementComponent;

UCLASS()
class UE5_TIMETRAVLEHUNTER_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	UPROPERTY(EditAnywhere, Category = "Animation | Action")
	UAnimMontage *Assasination_Vic;

	UPROPERTY(EditAnywhere, Category = "Animation | Action")
	UAnimMontage *MeleeAttack_Anim;

	UPROPERTY(EditAnywhere, Category = "Animation | Action")
	UAnimMontage *HitReaction_Tick_Anim;

	UPROPERTY(EditAnywhere, Category = "Animation | Action")
	UAnimMontage *HitReaction_KnockBack_Anim;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void PlayAssasination_Vic();
	void PlayMeleeAttack();
	void PlayHitReactionTik();
	void PlayHitReactionKnockBack();
protected:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	AEnemyCharacter *OwningCharacter;


	//======================================================
	//=					- Functionary -					   =
	//======================================================
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;

private:
	//======================================================
	//=					- Variables -					   =
	//======================================================
	//Character Reference
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Reference", meta = (AllowPrivateAccess = "true"))
	AEnemyCharacter *OwnerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Reference", meta = (AllowPrivateAccess = "true"))
	AEnemyCharacterController *OwnerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Reference", meta = (AllowPrivateAccess = "true"))
	UCharacterMovementComponent *OwnerCharacterMovement;

	// Character Movement Data
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Movement Data", meta = (AllowPrivateAccess = "true"))
	FVector CharacterVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Movement Data", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Movement Data", meta = (AllowPrivateAccess = "true"))
	bool bIsStrafe;

	// Character Locomotion State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion State", meta = (AllowPrivateAccess = "true"))
	bool bIsAccelation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion State", meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion State", meta = (AllowPrivateAccess = "true"))
	bool bIsRun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion State", meta = (AllowPrivateAccess = "true"))
	bool bIsAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion State", meta = (AllowPrivateAccess = "true"))
	bool bIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Locomotion State", meta = (AllowPrivateAccess = "true"))
	bool bIsHitCheck;

	// Character Rotation Data
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Rotation Data", meta = (AllowPrivateAccess = "true"))
	float MovementYawDelta;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterState|Rotation Data", meta = (AllowPrivateAccess = "true"))
	float MovementStartAngle;

	//======================================================
	//=					- Functionary -					   =
	//======================================================
	void InitAnimationInstance();
	void SetMovementData();
	bool SetMovementDirection(float MinValue, float MaxValue, bool Mincluding, bool Maxcluding, float &Direction) const;
};
