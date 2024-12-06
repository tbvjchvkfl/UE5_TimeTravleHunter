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
		DetermineLocomotionState();
		DetermineMovementDirection();
	}
}

void UPlayerAnimInstance::InitAnimationInstance()
{
	ActualGroundSpeed = 0.0f;
	MovementSpeed = 0.0f;
	CharacterVelocity = FVector::ZeroVector;
	PreviousLocation = FVector::ZeroVector;
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
	bIsSprint = OwnerController->bIsSprint;
	bIsParkour = OwnerController->bIsParkour;

	FRotator BaseAimRotation = OwnerCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(OwnerCharacter->GetVelocity());
	MovementYawDelta = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, BaseAimRotation).Yaw;

	SetRotationRate(0.0f, 500.0f);

	GEngine->AddOnScreenDebugMessage(66, 3, FColor::Green, FString::Printf(TEXT("SecMovementAngle : %f"), OwnerCharacterMovement->MaxAcceleration));
}

void UPlayerAnimInstance::DetermineLocomotionState()
{
	if (bIsWalk && !bIsInAir && bIsAccelation)
	{
		CharacterMovementState = ECharacterMovementState::WALK;
	}
	else if (!bIsWalk && !bIsInAir && bIsAccelation)
	{
		CharacterMovementState = ECharacterMovementState::JOG;
	}
	else if (bIsInAir)
	{
		CharacterMovementState = ECharacterMovementState::JUMP;
	}
	else if (bIsCrouch)
	{
		CharacterMovementState = ECharacterMovementState::CROUCH;
	}
	else
	{
		CharacterMovementState = ECharacterMovementState::IDLE;
	}
}

void UPlayerAnimInstance::SetMaxAccelAndPlayRate()
{
	OwnerCharacterMovement->MaxAcceleration = GetCurveValue(FName("Movement_Speed"));
	float ClampValue = FMath::Clamp(GetCurveValue(FName("MoveData_Speed")), 50.0f, 1000.0f);
	PlayRate = UKismetMathLibrary::SafeDivide(MovementSpeed, ClampValue);
}

void UPlayerAnimInstance::SetRotationRate(float MinLocomotionValue, float MaxLocomotionValue)
{
	float ClampedRotValue = UKismetMathLibrary::MapRangeClamped(GetCurveValue(FName("Movement_Rotation")), 0.0f, 1.0f, MinLocomotionValue, MaxLocomotionValue);
	OwnerCharacterMovement->RotationRate = FRotator(0.0f, ClampedRotValue, 0.0f);

	GEngine->AddOnScreenDebugMessage(33, 3, FColor::Green, FString::Printf(TEXT("Current Rotation Rate : %f, %f, %f"), OwnerCharacterMovement->RotationRate.Pitch, OwnerCharacterMovement->RotationRate.Yaw, OwnerCharacterMovement->RotationRate.Roll));

	OwnerCharacterMovement->bAllowPhysicsRotationDuringAnimRootMotion = false;
}

bool UPlayerAnimInstance::SetMovementDirection(float MinValue, float MaxValue, float &Direction) const
{
	if (OwnerCharacter)
	{
		float MovementAngle = CalculateDirection(OwnerCharacterMovement->GetLastInputVector(), OwnerCharacter->GetActorRotation());

		float SecMovementAngle = CalculateDirection(OwnerCharacter->GetLastMovementInputVector(), OwnerCharacter->GetActorRotation());

		GEngine->AddOnScreenDebugMessage(3, 3, FColor::Green, FString::Printf(TEXT("MovementAngle : %f"), MovementAngle));

		GEngine->AddOnScreenDebugMessage(4, 3, FColor::Green, FString::Printf(TEXT("SecMovementAngle : %f"), SecMovementAngle));

		if (UKismetMathLibrary::InRange_FloatFloat(MovementAngle, MinValue, MaxValue, true, true))
		{
			Direction = MovementAngle;
			return true;
		}
		Direction = 0.0f;
		return false;
	}
	return false;
}

void UPlayerAnimInstance::DetermineMovementDirection()
{
	if (SetMovementDirection(-180.0f, -135.0f, MovementStartAngle))
	{
		CharacterMovementDirection = ECharacterMovementDirection::TURNLEFT_180;
	}
	else if (SetMovementDirection(-135.0f, -45.0f, MovementStartAngle))
	{
		CharacterMovementDirection = ECharacterMovementDirection::TURNLEFT_90;
	}
	else if (SetMovementDirection(45.0f, 135.0f, MovementStartAngle))
	{
		CharacterMovementDirection = ECharacterMovementDirection::TURNRIGHT_90;
	}
	else if (SetMovementDirection(135.0f, 180.0f, MovementStartAngle))
	{
		CharacterMovementDirection = ECharacterMovementDirection::TURNRIGHT_180;
	}
	else
	{
		CharacterMovementDirection = ECharacterMovementDirection::FOWARD;
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