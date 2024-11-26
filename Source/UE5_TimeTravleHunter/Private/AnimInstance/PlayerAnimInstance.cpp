// Fill out your copyright notice in the Description page of Project Settings.

// GameFramework
#include "AnimInstance/PlayerAnimInstance.h"
#include "Controller/PlayerCharacterController.h"
#include "Character/Player/PlayerCharacter.h"

// Engine
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"



void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	OwnerController = Cast<APlayerCharacterController>(GetWorld()->GetFirstPlayerController());
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!OwnerController)
	{
		OwnerController = Cast<APlayerCharacterController>(GetWorld()->GetFirstPlayerController());
	}
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	}
	SetMovementData();
	DetermineLocomotionState();
	TrackLocomotionState(ECharacterMovementState::IDLE);
	TrackLocomotionState(ECharacterMovementState::WALK);
	TrackLocomotionState(ECharacterMovementState::JOG);
}

void UPlayerAnimInstance::SetMovementData()
{
	if (OwnerController)
	{
		CharacterVelocity = OwnerCharacter->GetCharacterMovement()->Velocity;
		MovementSpeed = CharacterVelocity.Size();
		MaxSpeed = OwnerCharacter->GetCharacterMovement()->GetMaxSpeed();
		bIsInAir = OwnerCharacter->GetCharacterMovement()->IsFalling();
		LastInputVector = OwnerCharacter->GetCharacterMovement()->GetLastInputVector();
		LastInputVector.GetClampedToSize(0.0f, 1.0f);

		bIsAccelation = OwnerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f;
		bIsCrouch = OwnerController->bIsCrouch;
		bIsWalk = OwnerController->bIsWalk;
		bIsSprint = OwnerController->bIsSprint;
		bIsAimming = OwnerController->bIsAimming;
		bIsParkour = OwnerController->bIsParkour;

		FRotator BaseAimRotation = OwnerCharacter->GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(OwnerCharacter->GetVelocity());
		MovementYawDelta = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, BaseAimRotation).Yaw;
	}
}

bool UPlayerAnimInstance::IsMovementWithInThresholds(float MinCurrentSpeed, float MinMaxSpeed, float MinInputAccelation) const
{
	if (MinCurrentSpeed <= MovementSpeed && MinMaxSpeed <= MaxSpeed && MinInputAccelation <= LastInputVector.Size())
	{
		return true;
	}
	return false;
}

void UPlayerAnimInstance::DetermineLocomotionState()
{
	if (!bIsInAir)
	{
		if (OwnerCharacter)
		{
			if (FVector::DotProduct(CharacterVelocity.GetSafeNormal(), OwnerCharacter->GetCharacterMovement()->GetCurrentAcceleration().GetSafeNormal()) < 0.5f)
			{
				CharacterMovementState = ECharacterMovementState::IDLE;
			}
			if (IsMovementWithInThresholds(1.0f, 300.0f, 0.5f))
			{
				CharacterMovementState = ECharacterMovementState::JOG;
			}
			else if (IsMovementWithInThresholds(1.0f, 0.0f, 0.01f))
			{
				CharacterMovementState = ECharacterMovementState::WALK;
			}
			else
			{
				CharacterMovementState = ECharacterMovementState::IDLE;
			}
		}
	}
}

void UPlayerAnimInstance::TrackLocomotionState(ECharacterMovementState LocomotionState)
{
	bool bHasFirstFlag = false;
	bool bHasSecondFlag = false;

	if (CharacterMovementState == LocomotionState)
	{
		if (!bHasFirstFlag)
		{
			bHasFirstFlag = true;
			bHasSecondFlag = false;
			if (LocomotionState == ECharacterMovementState::WALK)
			{
				UpdateOnWalkEntry();
			}
			if (LocomotionState == ECharacterMovementState::JOG)
			{
				UpdateOnJogEntry();
			}
		}
		if(LocomotionState != ECharacterMovementState::IDLE)
		{
			UpdateLocomotionValue();
		}
	}
	else
	{
		if (!bHasSecondFlag)
		{
			bHasSecondFlag = true;
			bHasFirstFlag = false;
		}
	}
}

void UPlayerAnimInstance::UpdateLocomotionValue()
{
	PlayRate = SafeDivide(MovementSpeed, FMath::Clamp(GetCurveValue(FName("MoveData_Speed")), 50.0f, 1000.0f));
}

void UPlayerAnimInstance::UpdateOnWalkEntry()
{
	if (MovementSpeed < 50.0f)
	{
		StartRotation = OwnerCharacter->GetActorRotation();
		PrimaryTargetRotation = LastInputVector.Rotation();
		SecondaryTargetRotation = PrimaryTargetRotation;

		FRotator DeltaRotation = PrimaryTargetRotation - SecondaryTargetRotation;
		WalkStartRotation = DeltaRotation.Yaw;
		bIsPlayWalkStart = true;
	}
	else
	{
		PrimaryTargetRotation = OwnerCharacter->GetActorRotation();
		SecondaryTargetRotation = PrimaryTargetRotation;
	}
}

void UPlayerAnimInstance::UpdateOnJogEntry()
{
	if (MovementSpeed < 200.0f)
	{
		StartRotation = OwnerCharacter->GetActorRotation();
		PrimaryTargetRotation = LastInputVector.Rotation();
		SecondaryTargetRotation = PrimaryTargetRotation;

		FRotator DeltaRotation = PrimaryTargetRotation - SecondaryTargetRotation;
		JogStartRotation = DeltaRotation.Yaw;
		bIsPlayJogStart = true;
	}
	else
	{
		PrimaryTargetRotation = OwnerCharacter->GetActorRotation();
		SecondaryTargetRotation = PrimaryTargetRotation;
	}
}

void UPlayerAnimInstance::PlayCrouchVaulting()
{
	if (CrouchVaulting_Anim)
	{
		Montage_Play(CrouchVaulting_Anim, 1.0f);
	}
}

void UPlayerAnimInstance::PlayNormalVaulting()
{
	if (NormalVaulting_Anim)
	{
		Montage_Play(NormalVaulting_Anim, 1.0f);
	}
}

void UPlayerAnimInstance::PlayHurdling()
{
	if (Hurdling_Anim)
	{
		Montage_Play(Hurdling_Anim, 1.0f);
	}
}

void UPlayerAnimInstance::PlayAssasination()
{
	if (Assasination_Anim)
	{
		Montage_Play(Assasination_Anim, 1.0f);
	}
}

float UPlayerAnimInstance::SafeDivide(float Numerator, float Denominator, float DefaultValue) const
{
	if (FMath::Abs(Denominator) > KINDA_SMALL_NUMBER)
	{
		return Numerator / Denominator;
	}
	else
	{
		return DefaultValue;
	}
}

