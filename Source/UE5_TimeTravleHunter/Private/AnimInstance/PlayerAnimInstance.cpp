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
	
	InitAnimationInstance();
}

void UPlayerAnimInstance::NativeBeginPlay()
{
	OwnerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	OwnerController = Cast<APlayerCharacterController>(GetWorld()->GetFirstPlayerController());
	OwnerCharacterMovement = OwnerCharacter->GetCharacterMovement();
	PreviousLocation = OwnerCharacter->GetActorLocation();
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
	if (OwnerCharacter && !OwnerCharacterMovement)
	{
		OwnerCharacterMovement = OwnerCharacter->GetCharacterMovement();
	}
	if (OwnerCharacter && OwnerController && OwnerCharacterMovement)
	{
		DeltaTime = DeltaSeconds;
		SetMovementData();
		
	}
}

void UPlayerAnimInstance::InitAnimationInstance()
{
	ActualGroundSpeed = 0.0f;
	MovementSpeed = 0.0f;
	MovementStartAngle = 0.0f;
	CharacterVelocity = FVector::ZeroVector;
	CharacterMovementState = ECharacterMovementState::IDLE;
	CharacterMovementDirection = ECharacterMovementDirection::FOWARD;
	DeltaTime = 0.0f;
	CanWalkStartSkip = false;
	CanJogStartSkip = false;
}

void UPlayerAnimInstance::SetMovementData()
{
	CharacterVelocity = OwnerCharacterMovement->Velocity;
	MovementSpeed = CharacterVelocity.Size();

	bIsInAir = OwnerCharacterMovement->IsFalling();
	bIsAccelation = OwnerCharacterMovement->GetCurrentAcceleration().Size() > 0.0f;
	bIsCrouch = OwnerController->bIsCrouch;
	bIsWalk = OwnerController->bIsWalk;
	bIsJog = OwnerController->bIsJog;
	bIsSprint = OwnerController->bIsSprint;
	bIsParkour = OwnerController->bIsParkour;

	FRotator BaseAimRotation = OwnerCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(OwnerCharacter->GetVelocity());
	MovementYawDelta = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, BaseAimRotation).Yaw;

	SetMaxAccelAndPlayRate();
	SetRotationRate(0.0f, 500.0f);
	DetermineMovementDirection();
}

void UPlayerAnimInstance::SetMaxAccelAndPlayRate()
{
	float ClampValue = FMath::Clamp(GetCurveValue(FName("MoveData_Speed")), 50.0f, 1000.0f);
	PlayRate = UKismetMathLibrary::SafeDivide(MovementSpeed, ClampValue);
}

void UPlayerAnimInstance::SetRotationRate(float MinLocomotionValue, float MaxLocomotionValue)
{
	float ClampedRotValue = UKismetMathLibrary::MapRangeClamped(GetCurveValue(FName("Movement_Rotation")), 0.0f, 1.0f, MinLocomotionValue, MaxLocomotionValue);
	OwnerCharacterMovement->RotationRate = FRotator(0.0f, ClampedRotValue, 0.0f);

	OwnerCharacterMovement->bAllowPhysicsRotationDuringAnimRootMotion = false;
}

bool UPlayerAnimInstance::SetMovementDirection(float MinValue, float MaxValue, bool Mincluding, bool Maxcluding, float &Direction) const
{
	if (OwnerCharacter)
	{
		float MovementAngle = CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetActorRotation());

		float ClampAngle = FMath::Clamp(MovementAngle, -180.0f, 180.0f);

		if (UKismetMathLibrary::InRange_FloatFloat(ClampAngle, MinValue, MaxValue, Mincluding, Maxcluding))
		{
			Direction = ClampAngle;
			return true;
		}
		Direction = 0.0f;
		return false;
	}
	return false;
}

void UPlayerAnimInstance::DetermineMovementDirection()
{
	if (MovementSpeed <= 5.0f)
	{
		if (SetMovementDirection(-180.0f, -135.0f, true, false, MovementStartAngle))
		{
			CharacterMovementDirection = ECharacterMovementDirection::TURNLEFT_180;
			return;
		}
		else if (SetMovementDirection(-135.0f, -45.0f, true, false, MovementStartAngle))
		{
			CharacterMovementDirection = ECharacterMovementDirection::TURNLEFT_90;
			return;
		}
		else if (SetMovementDirection(45.0f, 135.0f, false, true, MovementStartAngle))
		{
			CharacterMovementDirection = ECharacterMovementDirection::TURNRIGHT_90;
			return;
		}
		else if (SetMovementDirection(135.0f, 180.0f, false, true, MovementStartAngle))
		{
			CharacterMovementDirection = ECharacterMovementDirection::TURNRIGHT_180;
			return;
		}
		else
		{
			CharacterMovementDirection = ECharacterMovementDirection::FOWARD;
			return;
		}
	}
}

// Tick에서 실행
void UPlayerAnimInstance::FootIK(float DeltaTime)
{
}

// 거리 계산
TTuple<bool, float> UPlayerAnimInstance::CapsuleDistance(FName SocketName, ACharacter *Owner)
{
	return TTuple<bool, float>();
}

// LineTrace
TTuple<bool, float, FVector> UPlayerAnimInstance::FootLineTrace(FName SocketName, ACharacter *Owner)
{
	return TTuple<bool, float, FVector>();
}

void UPlayerAnimInstance::PlayCrouchVaulting()
{
	if (CrouchVaulting_Anim)
	{
		Montage_Play(CrouchVaulting_Anim, 1.0f);
	}
}

void UPlayerAnimInstance::PlayHurdling()
{
	if (Hurdling_Anim)
	{
		Montage_Play(Hurdling_Anim, 1.0f);
	}
}

void UPlayerAnimInstance::PlayClimbing()
{
	if (Climbing_Anim)
	{
		Montage_Play(Climbing_Anim, 1.0f);
	}
}

void UPlayerAnimInstance::PlayMantling()
{
	if (Mantling_Anim)
	{
		Montage_Play(Mantling_Anim, 1.0f);
	}
}

void UPlayerAnimInstance::PlayAssasination()
{
	if (Assasination_Anim)
	{
		Montage_Play(Assasination_Anim, 1.0f);
	}
}