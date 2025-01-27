// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "Character/AnimInstance/EnemyAnimInstance.h"
#include "Character/Enemy/EnemyCharacter.h"
#include "Character/Controller/EnemyCharacterController.h"

// Engine
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	OwningCharacter = Cast<AEnemyCharacter>(TryGetPawnOwner());

	InitAnimationInstance();
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<AEnemyCharacter>(TryGetPawnOwner());
	}
	if (OwnerCharacter && !OwnerController)
	{
		OwnerController = Cast<AEnemyCharacterController>(OwnerCharacter->GetController());
	}
	if (OwnerCharacter && !OwnerCharacterMovement)
	{
		OwnerCharacterMovement = OwnerCharacter->GetCharacterMovement();
	}
	if (OwnerCharacter && OwnerController && OwnerCharacterMovement)
	{
		SetMovementData();
	}
}

void UEnemyAnimInstance::InitAnimationInstance()
{
	MovementSpeed = 0.0f;
	MovementStartAngle = 0.0f;
	CharacterVelocity = FVector::ZeroVector;
	bIsStrafe = false;
	MovementSpeed = 0.0f;
}

void UEnemyAnimInstance::SetMovementData()
{
	CharacterVelocity = OwnerCharacterMovement->Velocity;
	MovementSpeed = OwnerCharacter->GetVelocity().Size();
	
	bIsInAir = OwnerCharacterMovement->IsFalling();
	bIsAccelation = OwnerCharacterMovement->GetCurrentAcceleration().Size() > 0.0f;
	bIsRun = OwnerController->bIsRun;
	bIsAttack = OwnerController->bIsAttack;
	bIsDead = OwnerController->bIsDead;
	bIsStrafe = OwnerController->bIsStrafe;
	bIsHitCheck = OwnerController->bIsHitCheck;

	MovementYawDelta = UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::MakeRotFromX(OwnerCharacter->GetVelocity()), OwnerCharacter->GetBaseAimRotation()).Yaw;
}

bool UEnemyAnimInstance::SetMovementDirection(float MinValue, float MaxValue, bool Mincluding, bool Maxcluding, float &Direction) const
{
	return false;
}

void UEnemyAnimInstance::PlayAssasination_Vic()
{
	if (Assasination_Vic)
	{
		Montage_Play(Assasination_Vic, 1.0f);
	}
}

void UEnemyAnimInstance::PlayMeleeAttack()
{
	if (MeleeAttack_Anim && !Montage_IsPlaying(MeleeAttack_Anim))
	{
		OwnerController->bIsAttack = true;
		Montage_Play(MeleeAttack_Anim, 1.0f);
	}
	OwnerController->bIsAttack = false;
}

void UEnemyAnimInstance::PlayHitReactionTik()
{
	if (HitReaction_Tick_Anim)
	{
		if (!Montage_IsPlaying(HitReaction_Tick_Anim))
		{
			Montage_Play(HitReaction_Tick_Anim, 1.2f);
		}
	}
}

void UEnemyAnimInstance::PlayHitReactionKnockBack()
{
	if (HitReaction_KnockBack_Anim)
	{
		if (!Montage_IsPlaying(HitReaction_KnockBack_Anim))
		{
			Montage_Play(HitReaction_KnockBack_Anim, 1.2f);
		}
	}
}
