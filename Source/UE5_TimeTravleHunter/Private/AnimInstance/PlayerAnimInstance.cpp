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

void UPlayerAnimInstance::NativePostEvaluateAnimation()
{
	
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

	CalculateActualSpeed();
	SetRotationRate(0.0f, 500.0f);

	GEngine->AddOnScreenDebugMessage(0, 3, FColor::Green, FString::Printf(TEXT("GetActorRotation : %f, %f, %f"), OwnerCharacter->GetActorRotation().Pitch, OwnerCharacter->GetActorRotation().Yaw, OwnerCharacter->GetActorRotation().Roll));

	auto InputVector = OwnerCharacterMovement->GetLastInputVector().GetClampedToSize(0.0f, 1.0f);
	auto PrimaryRot = UKismetMathLibrary::MakeRotFromX(InputVector);

	GEngine->AddOnScreenDebugMessage(1, 3, FColor::Green, FString::Printf(TEXT("GetActorRotation : %f, %f, %f"), PrimaryRot.Pitch, PrimaryRot.Yaw, PrimaryRot.Roll));

	float WalkStartAngle = UKismetMathLibrary::NormalizedDeltaRotator(PrimaryRot, OwnerCharacter->GetActorRotation()).Yaw;

	GEngine->AddOnScreenDebugMessage(2, 3, FColor::Green, FString::Printf(TEXT("WalkStartAngle : %f"), WalkStartAngle));
}

void UPlayerAnimInstance::DetermineLocomotionState()
{
	if (bIsWalk && !bIsInAir && MovementSpeed >= 10.0f)
	{
		CharacterMovementState = ECharacterMovementState::WALK;
	}
	else if (!bIsWalk && !bIsInAir && MovementSpeed >= 250.0f)
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

bool UPlayerAnimInstance::SetMovementDirection(float MinValue, float MaxValue, float &Direction) const
{
	if (OwnerCharacter)
	{
		float MovementAngle = CalculateDirection(OwnerCharacter->GetLastMovementInputVector(), OwnerCharacter->GetActorRotation());

		if (UKismetMathLibrary::InRange_FloatFloat(MovementAngle, MinValue, MaxValue, false, true))
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

void UPlayerAnimInstance::CalculateActualSpeed()
{
	FVector Cal = OwnerCharacter->GetActorLocation() - PreviousLocation;
	ActualGroundSpeed = Cal.Size() / GetWorld()->DeltaTimeSeconds;
	PreviousLocation = OwnerCharacter->GetActorLocation();

}

void UPlayerAnimInstance::SetRotationRate(float MinLocomotionSpeed, float MaxLocomotionSpeed)
{
	float ClampedRotValue = UKismetMathLibrary::MapRangeClamped(GetCurveValue(FName("SubMovementRotation")), 0.0f, 1.0f, MinLocomotionSpeed, MaxLocomotionSpeed);
	OwnerCharacterMovement->RotationRate = FRotator(0.0f, ClampedRotValue, 0.0f);

	GEngine->AddOnScreenDebugMessage(33, 3, FColor::Green, FString::Printf(TEXT("Current Rotation Rate : %f, %f, %f"), OwnerCharacterMovement->RotationRate.Pitch, OwnerCharacterMovement->RotationRate.Yaw, OwnerCharacterMovement->RotationRate.Roll));

	OwnerCharacterMovement->bAllowPhysicsRotationDuringAnimRootMotion = true;
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

//void UPlayerAnimInstance::NativeInitializeAnimation()
//{
//	Super::NativeInitializeAnimation();
//	OwnerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
//	OwnerController = Cast<APlayerCharacterController>(GetWorld()->GetFirstPlayerController());
//	if (OwnerCharacter)
//	{
//		OwnerCharacterMovement = OwnerCharacter->GetCharacterMovement();
//	}
//	CharacterMovementState = ECharacterMovementState::IDLE;
//	LocomotionStateData.StateMachineName = FName("");
//	LocomotionStateData.StateName = FName("");
//}
//
//void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
//{
//	Super::NativeUpdateAnimation(DeltaSeconds);
//	if (!OwnerController)
//	{
//		OwnerController = Cast<APlayerCharacterController>(GetWorld()->GetFirstPlayerController());
//	}
//	if (!OwnerCharacter)
//	{
//		OwnerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
//	}
//	if (OwnerCharacter && !OwnerCharacterMovement)
//	{
//		OwnerCharacterMovement = OwnerCharacter->GetCharacterMovement();
//	}
//	if (OwnerCharacter && OwnerController && OwnerCharacterMovement)
//	{
//		DeltaTime = DeltaSeconds;
//		SetMovementData();
//		DetermineLocomotionState();
//		if (CharacterMovementState == ECharacterMovementState::WALK)
//		{
//			TrackLocomotionState(CharacterMovementState);
//		}
//		else if (CharacterMovementState == ECharacterMovementState::JOG)
//		{
//			TrackLocomotionState(CharacterMovementState);
//		}
//	}
//	GEngine->AddOnScreenDebugMessage(1, 3, FColor::Red, FString::Printf(TEXT("Walk State Global Weight : %f"), WalkStateData.GetGlobalWeight(*this)));
//	GEngine->AddOnScreenDebugMessage(2, 3, FColor::Red, FString::Printf(TEXT("Walk State Weight : %f"), WalkStateData.GetWeight(*this)));
//}
//
//void UPlayerAnimInstance::NativePostEvaluateAnimation()
//{
//	UpdateCharacterRotation();
//	CanWalkStartSkip = false;
//	CanJogStartSkip = false;
//}
//
//void UPlayerAnimInstance::SetMovementData()
//{
//	CharacterVelocity = OwnerCharacterMovement->Velocity;
//	MovementSpeed = CharacterVelocity.Size();
//	LastInputVector = OwnerCharacterMovement->GetLastInputVector().GetClampedToSize(0.0f, 1.0f);
//
//	bIsInAir = OwnerCharacterMovement->IsFalling();
//	bIsAccelation = OwnerCharacterMovement->GetCurrentAcceleration().Size() > 0.0f;
//	bIsCrouch = OwnerController->bIsCrouch;
//	bIsWalk = OwnerController->bIsWalk;
//	bIsSprint = OwnerController->bIsSprint;
//	bIsParkour = OwnerController->bIsParkour;
//
//	FRotator BaseAimRotation = OwnerCharacter->GetBaseAimRotation();
//	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(OwnerCharacter->GetVelocity());
//	MovementYawDelta = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, BaseAimRotation).Yaw;
//}
//
//void UPlayerAnimInstance::DetermineLocomotionState()
//{
//	if (bIsWalk && !bIsInAir && MovementSpeed >= 10.0f)
//	{
//		CharacterMovementState = ECharacterMovementState::WALK;
//	}
//	else if (!bIsWalk && !bIsInAir && MovementSpeed >= 250.0f)
//	{
//		CharacterMovementState = ECharacterMovementState::JOG;
//	}
//	else if (bIsInAir)
//	{
//		CharacterMovementState = ECharacterMovementState::JUMP;
//	}
//	else if (bIsCrouch)
//	{
//		CharacterMovementState = ECharacterMovementState::CROUCH;
//	}
//	else
//	{
//		CharacterMovementState = ECharacterMovementState::IDLE;
//	}
//}
//
//void UPlayerAnimInstance::UpdatePlayRate()
//{
//	float ClampRate = FMath::Clamp(GetCurveValue(FName("MoveData_Speed")), 50.0f, 1000.0f);
//	PlayRate = UKismetMathLibrary::SafeDivide(MovementSpeed, ClampRate);
//}
//
//void UPlayerAnimInstance::TrackLocomotionState(ECharacterMovementState LocomotionState)
//{
//	bool bFirstCheck = false;
//	bool bSecondCheck = false;
//	if (LocomotionState == CharacterMovementState)
//	{
//		if (!bFirstCheck)
//		{
//			bFirstCheck = true;
//			bSecondCheck = false;
//			if (LocomotionState == ECharacterMovementState::WALK)
//			{
//				UpdateOnWalkEntry();
//			}
//			else if (LocomotionState == ECharacterMovementState::JOG)
//			{
//				UpdateOnJogEntry();
//			}
//		}
//		UpdatePlayRate();
//	}
//	else
//	{
//		if (!bSecondCheck)
//		{
//			bFirstCheck = false;
//			bSecondCheck = true;
//		}
//	}
//}
//
//void UPlayerAnimInstance::UpdateOnWalkEntry()
//{
//	if (MovementSpeed < 50.0f)
//	{
//		StartRotation = OwnerCharacter->GetActorRotation();
//		MainRotation = UKismetMathLibrary::MakeRotFromX(LastInputVector);
//		SubRotation = MainRotation;
//		WalkStartAngle = UKismetMathLibrary::NormalizedDeltaRotator(MainRotation, StartRotation).Yaw;
//		CanWalkStartSkip = false;
//	}
//	else
//	{
//		ResetRotation();
//	}
//}
//
//void UPlayerAnimInstance::UpdateOnJogEntry()
//{
//	if (MovementSpeed < 200.0f)
//	{
//		StartRotation = OwnerCharacter->GetActorRotation();
//		MainRotation = UKismetMathLibrary::MakeRotFromX(LastInputVector);
//		SubRotation = MainRotation;
//		JogStartAngle = UKismetMathLibrary::NormalizedDeltaRotator(MainRotation, StartRotation).Yaw;
//		CanJogStartSkip = false;
//	}
//	else
//	{
//		ResetRotation();
//	}
//}
//
//void UPlayerAnimInstance::UpdateCharacterRotation()
//{
//	if (LocomotionStateData.IsRelevant(*this) && !OwnerCharacter->HasAnyRootMotion())
//	{
//		switch (CharacterMovementState)
//		{
//			case ECharacterMovementState::WALK:
//			{
//				MainRotation = UKismetMathLibrary::RInterpTo_Constant(MainRotation, OwnerCharacterMovement->GetCurrentAcceleration().Rotation(), DeltaTime, 1000.0f);
//
//				SubRotation = UKismetMathLibrary::RInterpTo(SubRotation, MainRotation, DeltaTime, 10.0f);
//
//				float Calculate = UKismetMathLibrary::SafeDivide(GetCurveValue(FName("MoveData_WalkRotationDelta")), WalkStateData.GetGlobalWeight(*this));
//				OwnerCharacter->SetActorRotation(FRotator(SubRotation.Pitch, SubRotation.Yaw + Calculate, SubRotation.Roll));
//			}
//			break;
//			case ECharacterMovementState::JOG:
//			{
//				MainRotation = UKismetMathLibrary::RInterpTo_Constant(MainRotation, OwnerCharacterMovement->GetCurrentAcceleration().Rotation(), DeltaTime, 1000.0f);
//
//				SubRotation = UKismetMathLibrary::RInterpTo(SubRotation, MainRotation, DeltaTime, 10.0f);
//
//				float Calculate = UKismetMathLibrary::SafeDivide(GetCurveValue(FName("MoveData_JogRotationDelta")), JogStateData.GetGlobalWeight(*this));
//				OwnerCharacter->SetActorRotation(FRotator(SubRotation.Pitch, SubRotation.Yaw + Calculate, SubRotation.Roll));
//			}
//			break;
//		}
//	}
//	else
//	{
//		ResetRotation();
//	}
//}
//
//void UPlayerAnimInstance::ResetRotation()
//{
//	if (OwnerCharacter)
//	{
//		MainRotation = OwnerCharacter->GetActorRotation();
//		SubRotation = MainRotation;
//	}
//}